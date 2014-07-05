// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Copyright (c) 2014 Allan Simon <allan.simon@supinfo.com>
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#ifndef LINTHESIA_MIDI_EVENT_H
#define LINTHESIA_MIDI_EVENT_H

#include <string>
#include <iostream>

#include "note.h"
#include "midi_util.h"

/**
 * For the midi format, a "event" is represent by 3 bytes
 */
struct MidiEventSimple {

    MidiEventSimple() :
        status(0), byte1(0), byte2(0) {
    }

    MidiEventSimple(
        unsigned char s,
        unsigned char b1,
        unsigned char b2
    ) : 
    status(s), byte1(b1), byte2(b2) {
    }

    unsigned char status;
    unsigned char byte1;
    unsigned char byte2;
};

/**
 *
 */
class MidiEvent {
public:

    static MidiEvent read_from_stream(
        std::istream &stream, 
        unsigned char lastStatus,
        bool containsDeltaPulses = true
    );

    static MidiEvent build(const MidiEventSimple &simple);
    static MidiEvent NullEvent();

    /**
     * NOTE: There is a VERY good chance you don't want to use this directly.
     * The only reason it's not private is because the standard containers
     * require a default constructor.
     */
    MidiEvent() :
        status(0),
        data1(0),
        data2(0),
        tempoMsPerQuaterNote(0)
    { }

    /**
     * Returns true if the event could be expressed in a simple event.  (So, 
     * this will return false for Meta and SysEx events.)
     */
    bool get_simple_event(MidiEventSimple *simple) const;

    /**
     *
     */
    MidiEventType get_type() const;

    /**
     *
     */
    uint32_t get_delta_pulses() const {
        return deltaPulses;
    }

    /**
     * This is generally for internal Midi library use only.
     */
    void set_delta_pulses(uint32_t _deltaPulses) {
        deltaPulses = _deltaPulses;
    }

    /**
     *
     */
    void shift_note(int shiftAmount);

    /**
     *
     */
    NoteId get_note_number() const;

    /**
     * Returns a friendly name for this particular Note-On or Note-
     * Off event. (e.g. "A#2")  Returns empty string on other types
     * of events.
     */ static std::string get_note_name(NoteId noteNumber); 
    /**
     * Returns the "Program to change to" value if this is a Program
     * Change event, 0 otherwise.
     */
    int get_program_number() const;

    /**
     * Returns the "velocity" of a Note-On (or 0 if this is a Note Off event).
     * Returns -1 for other event types.
     */
    int get_note_velocity() const;

    /**
     * Check if the midi event is a 'playable' one
     * i.e a event we can send to the midi output
     */
    bool is_playable() const;

    /**
     * If it's the midi event of a note that starts
     */
    bool is_note_on() const;

    /**
     * if it's the midi event of a note that ends
     */
    bool is_note_off() const;

    /**
     *
     */
    void set_velocity(int velocity);

    /**
     * Returns which type of meta event this is (or
     * MetaEvent_Unknown if type() is not EventType_Meta).
     */
    MidiMetaEventType get_meta_type() const;

    /** 
     * Retrieve the tempo from a tempo meta event in microseconds
     * per quarter note.  (Non-meta-tempo events will throw an error).
     */
    uint32_t get_temps_in_ms_per_quater_note() const;

    /**
     * Convenience function: Is this the special End-Of-Track event
     */
    bool is_end() const;

    /**
     * Returns which channel this event operates on.  This is
     * only defined for standard MIDI events that require a
     * channel argument.
     */
    unsigned char get_channel() const;

    /**
     * Used to know if the event is on the percussion channel
     */
    bool is_percussion_channel() const;


    /**
     *
     */
    void set_channel(unsigned char channel);

    /**
     * Does this event type allow arbitrary text
     */
    bool has_text() const;

    /**
     * Returns the text content of the event (or empty-string if
     * this isn't a text event.)
     */
    std::string get_text() const;

    /**
     * Returns the status code of the MIDI event
     */
    unsigned char get_status_code() const {
        return status;
    }

private:
    void read_meta(std::istream &stream);
    void read_sys_ex(std::istream &stream);
    void read_standard(std::istream &stream);

    unsigned char status;
    unsigned char data1;
    unsigned char data2;
    uint32_t deltaPulses;

    unsigned char metaType;

    uint32_t tempoMsPerQuaterNote;
    std::string text;
};

#endif

