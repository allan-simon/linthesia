// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Copyright (c) 2014 Allan Simon <allan.simon@supinfo.com>
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#ifndef LINTHESIA_MIDI_H
#define LINTHESIA_MIDI_H

#include <cstdint>
#include <iostream>
#include <vector>

#include "note.h"
#include "midi_types.h"
#include "midi_track.h"

class MidiError;
class MidiEvent;

typedef std::vector<MidiTrack> MidiTrackList;

typedef std::vector<MidiEvent> MidiEventList;
typedef std::vector<std::pair<size_t, MidiEvent> > MidiEventListWithTrackId;

// NOTE: This library's MIDI loading and handling is destructive.  Perfect
//       1:1 serialization routines will not be possible without quite a
//       bit of additional work.
class Midi {

    public:
        /**
         *
         */
        static Midi read_from_file(const std::string &filename);

        /**
         *
         */
        static Midi read_from_stream(std::istream &stream);

        /**
         *
         */
        const MidiTrackList &get_tracks() const {
            return tracks;
        }

        /**
         *
         */
        const TranslatedNoteSet &get_notes() const {
            return translatedNotes;
        }

        /**
         *
         */
        microseconds_t get_song_position_in_ms() const {
            return songPositionInMs;
        }

        /**
         *
         */
        microseconds_t get_song_length_in_ms () const;

        /**
         *
         */
        microseconds_t get_dead_air_start_off_in_ms() const {
            return deadStartAirInMS;
        }


        /**
         * This doesn't include lead-in (so it's perfect for a progress bar).
         * (It is also clamped to [0.0, 1.0], so lead-in and lead-out won't
         * give any unexpected results.)
         */ 
        double get_song_percentage_complete() const;

        /**
         * This will report when the lead-out period is complete
         */
        bool is_song_over() const;
       
        /**
         *
         */
        unsigned int aggregate_events_remain() const;

        /**
         *
         */
        unsigned int aggregate_events_count() const;
       
        /**
         *
         */
        unsigned int aggregate_notes_remain() const;

        /**
         *
         */
        unsigned int aggregate_notes_count() const;

        /**
         *
         */
        MidiEventListWithTrackId update(microseconds_t deltaInMS);

        /**
         *
         */
        void reset(
            microseconds_t leadInInMs,
            microseconds_t leadOutInMs
        );


    private:
    
        const static uint32_t DEFAULT_BPM = 120;
        const static microseconds_t ONE_MIN_IN_MS = 60000000;
        const static microseconds_t DEFAULT_US_TEMPO = ONE_MIN_IN_MS / DEFAULT_BPM;


        /**
         * Convert a number of pulses into microsecond based
         * on tempo and number of pulse per 1/4 note
         */
        static microseconds_t pulses_to_ms(
            uint32_t pulses,
            microseconds_t tempo,
            uint16_t pulsesPerQuarterNote
        );
  

        /**
         * Constructor, private, as we instead use the "from_stream"
         * or "from_file" methods
         */
        Midi(): isInitialized(false), deadStartAirInMS(0) {
            reset(0,0);
        }

       /**
        * Note: This is O(n) where n is the number of tempo changes (across
        * all tracks) in the song up to the specified time.  Tempo changes
        * are usually a small number. (Almost always 0 or 1, going up to
        * maybe 30-100 in rare cases.)
        */
        microseconds_t get_event_pulse_in_ms(
            uint32_t eventPulses,
            uint16_t pulsesPerQuarterNote
        ) const;

        /**
         *
         */
        uint32_t find_first_note_pulse();

        /**
         *
         */
        void build_tempo_track();

        /**
         *
         */
        void translate_notes(
            const NoteSet &notes,
            uint16_t pulsesPerQuarterNote
        );

        bool isInitialized;
        TranslatedNoteSet translatedNotes;

        // Position can be negative (for lead-in).
        microseconds_t songPositionInMs;
        microseconds_t baseSongLengthInMs;

        microseconds_t leadOutInMs;
        microseconds_t deadStartAirInMS;

        bool isFirstUpdateAfterReset;
        double playbackSpeed;
        MidiTrackList tracks;

};

#endif

