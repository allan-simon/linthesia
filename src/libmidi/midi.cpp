// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Copyright (c) 2014 Allan Simon <allan.simon@supinfo.com>
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#include "midi.h"
#include "midi_event.h"
#include "midi_track.h"
#include "midi_util.h"

#include <fstream>
#include <map>
#include <utility>
#include <stdint.h>

using namespace std;

/**
 *
 */
Midi Midi::read_from_file(const string &filename) {

    fstream file(
        filename.c_str(),
        ios::in | ios::binary
    );

    if (!file.good()) {
        throw MidiError(MidiError_BadFilename);
    }

    Midi midi;

    try {
        midi = read_from_stream(file);
    } catch (const MidiError &e) {
        // Close our file resource before handing the error up
        file.close();
        throw e;
    }
    return midi;
}

/**
 *
 */
Midi Midi::read_from_stream(istream &stream) {
    Midi midi;

    // header_id is always "MThd" by definition
    const static string MIDI_FILE_HEADER = "MThd";
    const static uint32_t MIDI_FILE_HEADER_LENGTH = 4;
    const static string RIFF_FILE_HEADER = "RIFF";

    // I could use (MIDI_FILE_HEADER.length() + 1), but then this has to be
    // dynamically allocated.  More hassle than it's worth.  MIDI is well
    // defined and will always have a 4-byte header.  We use 5 so we get
    // free null termination.
    char     headerId[5] = { 0, 0, 0, 0, 0 };
    uint32_t headerLength;
    uint16_t format;
    uint16_t trackCount;
    uint16_t timeDivision;

    stream.read(
        headerId,
        static_cast<streamsize>(MIDI_FILE_HEADER_LENGTH)
    );
    string header(headerId);

    if (header != MIDI_FILE_HEADER) {
        if (header != RIFF_FILE_HEADER) {
            throw MidiError(MidiError_UnknownHeaderType);
        }

        // We know how to support RIFF files
        uint32_t throwAway;
        stream.read(reinterpret_cast<char*>(&throwAway), sizeof(uint32_t)); // RIFF length
        stream.read(reinterpret_cast<char*>(&throwAway), sizeof(uint32_t)); // "RMID"
        stream.read(reinterpret_cast<char*>(&throwAway), sizeof(uint32_t)); // "data"
        stream.read(reinterpret_cast<char*>(&throwAway), sizeof(uint32_t)); // data size

        // Call this recursively, without the RIFF header this time
        return read_from_stream(stream);
    }

    stream.read(reinterpret_cast<char*>(&headerLength), sizeof(uint32_t));
    stream.read(reinterpret_cast<char*>(&format),        sizeof(uint16_t));
    stream.read(reinterpret_cast<char*>(&trackCount),   sizeof(uint16_t));
    stream.read(reinterpret_cast<char*>(&timeDivision), sizeof(uint16_t));

    if (stream.fail())
        throw MidiError(MidiError_NoHeader);

    // Chunk Size is always 6 by definition
    const static unsigned int MIDI_HEADER_CHUNK_LENGTH = 6;

    headerLength = big_to_system32(headerLength);
    if (headerLength != MIDI_HEADER_CHUNK_LENGTH) {
        throw MidiError(MidiError_BadHeaderSize);
    }

    enum MidiFormat {
        MidiFormat0 = 0,
        MidiFormat1 = 1,
        MidiFormat2 = 2
    };

    format = big_to_system16(format);
    if (format == MidiFormat2) {
        // MIDI 0: All information in 1 track
        // MIDI 1: Multiple tracks intended to be played simultaneously
        // MIDI 2: Multiple tracks intended to be played separately
        //
        // We do not support MIDI 2 at this time
        throw MidiError(MidiError_Type2MidiNotSupported);
    }

    trackCount = big_to_system16(trackCount);
    if (format == 0 && trackCount != 1) {
        // MIDI 0 has only 1 track by definition
        throw MidiError(MidiError_BadType0Midi);
    }

    // Time division can be encoded two ways based on a bit-flag:
    // - pulses per quarter note (15-bits)
    // - SMTPE frames per second (7-bits for SMPTE frame count and 8-bits for clock ticks per frame)
    timeDivision = big_to_system16(timeDivision);
    bool inSMPTE = ((timeDivision & 0x8000) != 0);

    if (inSMPTE) {
        throw MidiError(MidiError_SMTPETimingNotImplemented);
    }

    // We ignore the possibility of SMPTE timing, so we can
    // use the time division value directly as PPQN.
    uint16_t pulsesPerQuarterNote = timeDivision;

    // Read in our tracks
    for (int i = 0; i < trackCount; ++i) {
        midi.tracks.push_back(MidiTrack::read_from_stream(stream));
    }

    midi.build_tempo_track();

    // Tell our tracks their IDs
    for (unsigned i = 0; i < trackCount; ++i) {
        midi.tracks[i].set_track_id(i);
    }

    // Translate each track's list of notes and list
    // of events into microseconds.
    for (auto& track : midi.tracks) {
        track.reset(); midi.translate_notes( track.get_notes(), pulsesPerQuarterNote);

        MidiEventMsList eventsMs;
        for (auto eventPulse: track.get_event_pulses()) {
            eventsMs.push_back(
                midi.get_event_pulse_in_ms(
                    eventPulse,
                    pulsesPerQuarterNote
                )
            );
        }
        track.set_events_ms(eventsMs);
    }


    midi.isInitialized = true;

    // Just grab the end of the last note to find out how long the song is
    midi.baseSongLengthInMs = midi.translatedNotes.rbegin()->end;

    // Eat everything up until *just* before the first note event
    midi.deadStartAirInMS = midi.get_event_pulse_in_ms(
        midi.find_first_note_pulse(),
        pulsesPerQuarterNote
    );
    midi.deadStartAirInMS -= 1;

    return midi;
}

// NOTE: This is required for much of the other functionality provided
// by this class, however, this causes a destructive change in the way
// the MIDI is represented internally which means we can never save the
// file back out to disk exactly as we loaded it.
//
// This adds an extra track dedicated to tempo change events.  Tempo events
// are extracted from every other track and placed in the new one.
//
// This allows quick(er) calculation of wall-clock event times
void Midi::build_tempo_track() {
    // This map will help us get rid of duplicate events if
    // the tempo is specified in every track (as is common).
    //
    // It also does sorting for us so we can just copy the
    // events right over to the new track.
    map<uint32_t, MidiEvent> tempoEvents;

    for (auto& track : tracks) {
        size_t total = track.get_events().size();
        for (size_t i = 0; i < total; ++i) {
            MidiEvent event = track.get_events()[i];
            uint32_t eventPulses = track.get_event_pulses()[i];

            if (
                event.get_type() != MidiEventType_Meta ||
                event.get_meta_type() != MidiMetaEvent_TempoChange
            ) {
                continue;
            }

            // Pull tempo event out of both lists
            //
            // Vector is kind of a hassle this way -- we have to
            // walk an iterator to that point in the list because
            // erase MUST take an iterator... but erasing from a
            // list invalidates iterators.  bleah.
            //TODO: certainly possible to get something better

            MidiEventList::iterator eventToErase = track.get_events().begin();
            MidiEventPulsesList::iterator eventPulseToErase = track.get_event_pulses().begin();
            for (size_t j = 0; j < i; ++j) {
                ++eventToErase;
                ++eventPulseToErase;
            }

            track.get_events().erase(eventToErase);
            track.get_event_pulses().erase(eventPulseToErase);

            // Adjust next event's delta time
            if (track.get_events().size() > i) {
                // (We just erased the element at i, so
                // now i is pointing to the next element)
                uint32_t nextDelta = track.get_events()[i].get_delta_pulses();

                track.get_events()[i].set_delta_pulses(
                    event.get_delta_pulses() + nextDelta
                );
            }

            // We have to roll i back for the next loop around
            --i;

            // Insert our newly stolen event into the auto-sorting map
            tempoEvents[eventPulses] = event;

        }

    }

    // Create a new track (always the last track in the track list)
    tracks.push_back(MidiTrack::create_blank_track());

    MidiEventList &tempoTrackEvents = tracks.back().get_events();
    MidiEventPulsesList &tempoTrackEventPulses = tracks.back().get_event_pulses();

    // Copy over all our tempo events
    uint32_t previousAbsolutePulses = 0;
    for (auto& tempoEvent : tempoEvents) {
        uint32_t absolutePulses = tempoEvent.first;
        MidiEvent event = tempoEvent.second;

        // Reset each of their delta times while we go
        event.set_delta_pulses(absolutePulses - previousAbsolutePulses);
        previousAbsolutePulses = absolutePulses;

        // Add them to the track
        tempoTrackEventPulses.push_back(absolutePulses);
        tempoTrackEvents.push_back(event);

    };
}

/**
 *
 */
uint32_t Midi::find_first_note_pulse() {
    uint32_t firstNotePulse = 0;

    // Find the very last value it could ever possibly be, to start with
    for (auto track: tracks) {
        if (track.get_event_pulses().size() == 0) {
            continue;
        }
        uint32_t pulses = track.get_event_pulses().back();
        if (pulses > firstNotePulse) {
            firstNotePulse = pulses;
        }
    };

    // Now run through each event in each track looking for the very
    // first note_on event
    for (auto track: tracks) {
        for (size_t eventId = 0; eventId < track.get_events().size(); ++eventId) {
            if (track.get_events()[eventId].get_type() != MidiEventType_NoteOn) {
                continue;
            }
            uint32_t notePulse = track.get_event_pulses()[eventId];
            if (notePulse < firstNotePulse) {
                firstNotePulse = notePulse;
            }
            // We found the first note event in this
            // track.  No need to keep searching.
            break;
        }
    }

    return firstNotePulse;
}

/**
 *
 */
microseconds_t Midi::pulses_to_ms(
    uint32_t pulses,
    microseconds_t tempo,
    uint16_t pulsesPerQuarterNote
) {
    // Here's what we have to work with:
    //   pulses is given
    //   tempo is given (units of microseconds/quarter_note)
    //   (pulses/quarter_note) is given as a constant in this object file
    const double quarterNotes =
        static_cast<double>(pulses) /
        static_cast<double>(pulsesPerQuarterNote);

    const double microseconds = quarterNotes * static_cast<double>(tempo);

    return static_cast<microseconds_t>(microseconds);
}

/**
 *
 */
microseconds_t Midi::get_event_pulse_in_ms(
    uint32_t eventPulses,
    uint16_t pulsesPerQuarterNote
) const {
    if (tracks.size() == 0) {
        return 0;
    }

    const MidiTrack &tempoTrack = tracks.back();

    microseconds_t runningResult = 0;

    bool hit = false;
    uint32_t lastTempoEventPulses = 0;
    microseconds_t runningTempo = DEFAULT_US_TEMPO;

    for (size_t i = 0; i < tempoTrack.get_events().size(); ++i) {
        uint32_t tempoEventPulses = tempoTrack.get_events_pulses()[i];

        // If the time we're asking to convert is still beyond
        // this tempo event, just add the last time slice (at
        // the previous tempo) to the running wall-clock time.
        uint32_t deltaPulses = 0;
        if (eventPulses > tempoEventPulses) {
            deltaPulses = tempoEventPulses - lastTempoEventPulses;
        } else {
            hit = true;
            deltaPulses = eventPulses - lastTempoEventPulses;
        }

        runningResult += pulses_to_ms(
            deltaPulses,
            runningTempo,
            pulsesPerQuarterNote
        );

        // If the time we're calculating is before the tempo event we're
        // looking at, we're done.
        if (hit) {
            break;
        }

        runningTempo = tempoTrack.get_events()[i].get_temps_in_ms_per_quater_note();
        lastTempoEventPulses = tempoEventPulses;
    }

    // The requested time may be after the very last tempo event
    if (!hit) {
        uint32_t remainingPulses = eventPulses - lastTempoEventPulses;
        runningResult += pulses_to_ms(
            remainingPulses,
            runningTempo,
            pulsesPerQuarterNote
        );
    }

    return runningResult;
}

/**
 *
 */
void Midi::reset(
    microseconds_t leadInMicroseconds,
    microseconds_t leadOutMicroseconds
) {
    leadOutInMs = leadOutMicroseconds;
    songPositionInMs = deadStartAirInMS - leadInMicroseconds;
    isFirstUpdateAfterReset = true;

    for (auto& track: tracks) {
        track.reset();
    }

}

/**
 *
 */
void Midi::translate_notes(
    const NoteSet &notes,
    uint16_t pulsesPerQuarterNote
) {

    for (auto note: notes) {
        TranslatedNote trans;

        trans.noteId = note.noteId;
        trans.trackId = note.trackId;
        trans.channel = note.channel;
        trans.velocity = note.velocity;
        trans.start = get_event_pulse_in_ms(
            note.start,
            pulsesPerQuarterNote
        );
        trans.end = get_event_pulse_in_ms(
            note.end,
            pulsesPerQuarterNote
        );
        translatedNotes.insert(trans);
    }

}

/**
 *
 */
MidiEventListWithTrackId Midi::update(microseconds_t deltaMicroseconds) {
    MidiEventListWithTrackId aggregatedEvents;
    if (!isInitialized) {
        return aggregatedEvents;
    }

    songPositionInMs += deltaMicroseconds;
    if (isFirstUpdateAfterReset) {
        deltaMicroseconds += songPositionInMs;
        isFirstUpdateAfterReset = false;
    }

    if (deltaMicroseconds == 0) {
        return aggregatedEvents;
    }

    if (deltaMicroseconds > songPositionInMs) {
        deltaMicroseconds = songPositionInMs;
    }

    const size_t trackCount = tracks.size();
    for (size_t i = 0; i < trackCount; ++i) {
        MidiEventList trackEvents = tracks[i].update(deltaMicroseconds);

        const size_t eventCount = trackEvents.size();
        for (size_t j = 0; j < eventCount; ++j) {

            std::pair<size_t, MidiEvent> eventPair(i, trackEvents[j]);
            aggregatedEvents.insert(
                aggregatedEvents.end(),
                eventPair
            );
        }
    }

    return aggregatedEvents;
}

/**
 *
 */
microseconds_t Midi::get_song_length_in_ms() const {
    if (!isInitialized) {
        return 0;
    }

    return baseSongLengthInMs - deadStartAirInMS;
}

unsigned int Midi::aggregate_events_remain() const {
    if (!isInitialized) {
        return 0;
    }
    unsigned int aggregate = 0;
    for (auto track : tracks) {
        aggregate += track.aggregate_events_remain();
    }
    return aggregate;
}

/**
 *
 */
unsigned int Midi::aggregate_notes_remain() const {
    if (!isInitialized) {
        return 0;
    }

    unsigned int aggregate = 0;
    for (auto track: tracks) {
        aggregate += track.aggregate_notes_remain();
    }

    return aggregate;
}

/**
 *
 */
unsigned int Midi::aggregate_events_count() const {
    if (!isInitialized) {
        return 0;
    }

    unsigned int aggregate = 0;
    for (auto track : tracks) {
        aggregate += track.aggregate_events_count();
    }

    return aggregate;
}

unsigned int Midi::aggregate_notes_count() const {
    if (!isInitialized) {
        return 0;
    }

    unsigned int aggregate = 0;
    for (auto track : tracks) {
        aggregate += track.aggregate_notes_count();
    }

    return aggregate;
}

/**
 *
 */
double Midi::get_song_percentage_complete() const {
    if (!isInitialized) {
        return 0.0;
    }
    const double pos = static_cast<double>(songPositionInMs - deadStartAirInMS);
    const double len = static_cast<double>(get_song_length_in_ms());

    if (pos < 0) {
        return 0.0;
    }

    if (len == 0) {
        return 1.0;
    }

    return min( (pos / len), 1.0 );
}

/**
 *
 */
bool Midi::is_song_over() const {
    if (!isInitialized) {
        return true;
    }

    return
        (songPositionInMs - deadStartAirInMS) >=
        get_song_length_in_ms() + leadOutInMs;
}
