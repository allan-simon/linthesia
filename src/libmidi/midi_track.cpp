// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#include "midi_track.h"
#include "midi_event.h"
#include "midi_util.h"
#include "midi.h"

#include <stdint.h>
#include <sstream>
#include <string>
#include <map>

using namespace std;

/**
 *
 */
struct NoteInfo {

   int velocity;
   unsigned char channel;
   unsigned long pulses;
};

/**
 *
 */
MidiTrack MidiTrack::read_from_stream(std::istream &stream) {
    // Verify the track header
    const static string MIDI_TRACK_HEADER = "MTrk";
    const static uint32_t MIDI_TRACK_HEADER_LENGTH = 4;

    // I could use (MIDI_TRACK_HEADER.length() + 1), but then this has to be
    // dynamically allocated.  More hassle than it's worth.  MIDI is well
    // defined and will always have a 4-byte header.  We use 5 so we get
    // free null termination.
    char headerId[5] = { 0, 0, 0, 0, 0 };
    uint32_t trackLength;

    stream.read(headerId, static_cast<streamsize>(MIDI_TRACK_HEADER_LENGTH));
    stream.read(reinterpret_cast<char*>(&trackLength), sizeof(uint32_t));

    if (stream.fail()) {
        throw MidiError(MidiError_TrackHeaderTooShort);
    }

    string header(headerId);

    if (header != MIDI_TRACK_HEADER) {
        throw MidiError_BadTrackHeaderType;
    }
    // Pull the full track out of the file all at once -- there is an
    // End-Of-Track event, but this allows us handle malformed MIDI a
    // little more gracefully.
    trackLength = big_to_system32(trackLength);
    char *buffer = new char[trackLength + 1];
    buffer[trackLength] = 0;

    stream.read(buffer, trackLength);
    if (stream.fail()) {
        delete[] buffer;
        throw MidiError(MidiError_TrackTooShort);
    }

    // We have to jump through a couple hoops because istringstream
    // can't handle binary data unless constructed through an std::string.
    string bufferString(buffer, trackLength);
    istringstream eventStream(bufferString, ios::binary);
    delete[] buffer;

    MidiTrack track;

    // Read events until we run out of track
    unsigned char lastStatus = 0;
    uint32_t currentPulseCount = 0;
    while (eventStream.peek() != char_traits<char>::eof()) {
        MidiEvent event = MidiEvent::read_from_stream(eventStream, lastStatus);
        lastStatus = event.get_status_code();

        track.add_event(event);

        currentPulseCount += event.get_delta_pulses();
        track.add_event_pulse(currentPulseCount);
    }

    track.build_note_set();
    track.discover_instrument();

    return track;
}

/**
 *
 */
void MidiTrack::build_note_set() {
    noteSet.clear();

    // Keep a list of all the notes currently "on" (and the pulse that
    // it was started).  On a note_on event, we create an element.  On
    // a note_off event we check that an element exists, make a "Note",
    // and remove the element from the list.  If there is already an
    // element on a note_on we both cap off the previous "Note" and
    // begin a new one.
    //
    // A note_on with velocity 0 is a note_off
    map<NoteId, NoteInfo> activeNotes;

    for (size_t i = 0; i < events.size(); ++i) {
        const MidiEvent &event = events[i];

        if (
            event.get_type() != MidiEventType_NoteOn &&
            event.get_type() != MidiEventType_NoteOff
        ) {
            continue;
        }

        bool on = (
            event.get_type() == MidiEventType_NoteOn &&
            event.get_note_velocity() > 0
        );

        NoteId id = event.get_note_number();

        // Check for an active note
        map<NoteId, NoteInfo>::iterator findRet = activeNotes.find(id);
        bool activeEvent = (findRet !=  activeNotes.end());

        // Close off the last event if there was one
        if (activeEvent) {
            Note note;
            note.start = findRet->second.pulses;
            note.end = eventPulses[i];
            note.noteId = id;
            note.channel = findRet->second.channel;
            note.velocity = findRet->second.velocity;

            // NOTE: This must be set at the next level up.  The track
            // itself has no idea what its index is.
            note.trackId = 0;

            // Add a note and remove this NoteId from the active list
            noteSet.insert(note);
            activeNotes.erase(findRet);
        }

        // We've handled any active events.  If this was a note_off we're done.
        if (!on) {
            continue;
        }

        // Add a new active event
        NoteInfo info;
        info.channel = event.get_channel();
        info.velocity = event.get_note_velocity();
        info.pulses = eventPulses[i];

        activeNotes[id] = info;
    }

    if (!activeNotes.empty()) {
        // LOGTODO!

        // This is mostly non-critical.
        //
        // Erroring out would be needlessly restrictive against
        // promiscuous MIDI files.  As-is, a note just won't be
        // inserted if it isn't closed properly.
    }
}

/**
 *
 */
void MidiTrack::discover_instrument() {
    // Default to Program 0 per the MIDI Standard
    instrumentId = 0;

    // Check to see if any/all of the notes
    // in this track use Channel 10.
    bool anyNoteUsesPercussion = false;
    bool anyNoteDoesNotUsePercussion = false;

    //we do a first pass to check the special cases
    // "percussion" and "various" (mixed with percussion)
    for (auto event : events) {

        if (event.get_type() != MidiEventType_NoteOn) {
            continue;
        }

        if (event.is_percussion_channel()) {
            anyNoteUsesPercussion = true;
        }

        if (!event.is_percussion_channel()) {
            anyNoteDoesNotUsePercussion = true;
        };
    }

    if (anyNoteUsesPercussion && !anyNoteDoesNotUsePercussion) {
        instrumentId = INSTRUMENT_ID_PERCUSSION;
        return;
    }

    if (anyNoteUsesPercussion && anyNoteDoesNotUsePercussion) {
        instrumentId = INSTRUMENT_ID_VARIOUS;
        return;
    }

    // now we look for "normal" instruments

    bool instrumentFound = false;
    for (auto event : events) {

        if (event.get_type() != MidiEventType_ProgramChange) {
            continue;
        }

        // If we've already hit a different instrument in this
        // same track, just tag it as "various" and exit early
        //
        // Also check that the same instrument isn't just set
        // multiple times in the same track
        if (instrumentFound && instrumentId != event.get_program_number()) {
            instrumentId = INSTRUMENT_ID_VARIOUS;
            return;
        }

        instrumentId = event.get_program_number();
        instrumentFound = true;
    }
}

/**
 *
 */
void MidiTrack::set_track_id(size_t _trackId) {
    trackId = _trackId;
    // we copy the old note set
    // we delete the original
    NoteSet old = noteSet;
    noteSet.clear();

    // and we create a new one by putting
    // the note one by one and changing their
    // track id

    // TODO: there's certainly a way to do that
    // "in place"
    for (auto note : old) {
        note.trackId = trackId;
        noteSet.insert(note);
    }
}

/**
 *
 */
size_t MidiTrack::get_track_id() const {
    return trackId;
}

/**
 *
 */
void MidiTrack::reset() {
    runningMs = 0;
    // TODO : BIG HACK to please the compiler
    // but basically here we have an integer overflow
    // in the original program, lastEvent was set to "-1"
    // which is normally not possible (I guess it was then set to
    // the highest uint32
    // however it's working fine, because it is always used in addition with +1
    // long story short: it should be possible to set it to "0" and remove the +1
    uint32_t one = 1;
    lastEvent = 0 - one;
    notesRemaining = static_cast<unsigned int>(noteSet.size());
}

/**
 *
 */
MidiEventList MidiTrack::update(microseconds_t deltaMs) {
    runningMs += deltaMs;

    MidiEventList tempEvents;

    //TODO: case of #3 (lastEvent + 1)
    for (size_t i = lastEvent + 1; i < events.size(); ++i) {
        if (eventsMs[i] > runningMs) {
            break;
        }
        tempEvents.push_back(events[i]);
        lastEvent = static_cast<uint32_t>(i);

        // if it's an actual note
        // TODO: this test is certainly repeated a lot
        if (
            events[i].get_type() == MidiEventType_NoteOn &&
            events[i].get_note_velocity() > 0
        ) {
            notesRemaining--;
        }
    }

    return tempEvents;
}

