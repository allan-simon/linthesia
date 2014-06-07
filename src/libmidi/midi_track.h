// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Copyright (c) 2014 Allan Simon <allan.simon@supinfo.com>
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#ifndef LINTHESIA_MIDI_TRACK_H
#define LINTHESIA_MIDI_TRACK_H

#include <vector>
#include <iostream>

#include "note.h"
#include "midi_util.h"
#include "midi_event.h"


typedef std::vector<MidiEvent> MidiEventList;
typedef std::vector<uint32_t> MidiEventPulsesList;
typedef std::vector<microseconds_t> MidiEventMsList;

/**
 *
 */
class MidiTrack {
public:

    static MidiTrack read_from_stream(std::istream &stream);
    static MidiTrack create_blank_track() {
        return MidiTrack();
    }

    /**
     *
     */
    MidiEventList &get_events() {
        return events;
    }

    /**
     *
     */
    MidiEventPulsesList &get_event_pulses() {
        return eventPulses;
    }

    /**
     *
     */
    MidiEventMsList &get_events_ms() {
        return eventsMs;
    }

    //TODO: my understanding of C++ is low but why do we have two versions
    //of these getters a const one and non-const one ?

    /**
     *
     */
    const MidiEventList &get_events() const {
        return events;
    }
    /**
     *
     */
    const MidiEventPulsesList &get_events_pulses() const {
        return eventPulses;
    }

    /**
     *
     */
    const MidiEventMsList &get_events_ms() const {
        return eventsMs;
    }


    void set_events_ms(const MidiEventMsList &_eventsMs) {
        eventsMs = _eventsMs; 
    }

    /**
     * Get the name of the instrument playing this track
     */
    const std::string get_instrument_name () const {
        return INSTRUMENT_NAMES[instrumentId];
    }

    /**
     *
     */
    bool is_percussion () const {
        return instrumentId == INSTRUMENT_ID_PERCUSSION;
    }

    /**
     *
     */
    const NoteSet get_notes() const {
        return noteSet;
    }

    /**
     *
     */
    void set_track_id (size_t trackId);

    /**
     * Reports whether this track contains any Note-On MIDI events
     * (vs. just being an information track with a title or copyright)
     */
    bool has_notes() const {
        return noteSet.size() > 0;
    
    }

    /**
     *
     */
    void reset();

    /**
     *
     */
    MidiEventList update(microseconds_t deltaInMS);
   
    /**
     *
     */
    unsigned int aggregate_events_remain() const {
        //TODO: case of #3 (lastEvent + 1)
        return static_cast<unsigned int>(events.size() - (lastEvent + 1));
    }

    /**
     *
     */
    unsigned int aggregate_events_count() const {
        return static_cast<unsigned int>(events.size());
    }
   
    /**
     *
     */
    unsigned int aggregate_notes_remain() const {
        return notesRemaining;
    }

    /**
     *
     */
    unsigned int aggregate_notes_count() const {
        return static_cast<unsigned int>(noteSet.size());
    }

private:

    /**
     *
     */
    void add_event(const MidiEvent& event) {
        events.push_back(event);
    }

    /**
     *
     */
    void add_event_pulse(const uint32_t eventPulse) {
        eventPulses.push_back(eventPulse);
    }



    /**
     *
     */
    MidiTrack() : instrumentId(0) {
        reset();
    }

    void build_note_set();
    void discover_instrument();


    MidiEventList events;
    MidiEventPulsesList eventPulses;
    MidiEventMsList eventsMs;

    NoteSet noteSet;

    int instrumentId;

    microseconds_t runningMs;
    uint32_t lastEvent;
    unsigned int notesRemaining;
};

#endif

