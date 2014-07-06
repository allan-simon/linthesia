#ifndef LINTHESIA_MIDI_IN_H
#define LINTHESIA_MIDI_IN_H

#include <RtMidi.h>

#include "libmidi/midi.h"

class MidiEvent;

namespace linthesia {

/**
 * Used to get input MIDI event (from MIDI keyboard for example)
 */
class MidiIn {

    public:
        MidiIn(unsigned _chosenPort = 0);

        /**
         * Get the next MIDI in the buffer, return a "NullEvent" instance
         * if buffer is empty
         */
        MidiEvent read();

        /**
         * Get all the MIDI "note" event currently in input buffer, return a empty
         * list if no "note" midi event available
         */
        MidiEventList readAllNotes();

        /**
         * Change the output port use when writing midi event
         */
        void switchNextPort();
        void switchPrevPort();

        /**
         *
         */
        std::string getCurrentPortName();

        /**
         * Open the midi output, so that writing event actually goes
         * somewhere
         */
        void open();

        /**
         * Close the midi output, used to "stop" a song from being played
         * and clean notes that are still "on"
         */
        void close();

    private:
        RtMidiIn  rtMidiIn;

        unsigned chosenPort;
};


} //
#endif
