#ifndef LINTHESIA_MIDI_IN_H
#define LINTHESIA_MIDI_IN_H

#include <RtMidi.h>

class MidiEvent;

namespace linthesia {

/**
 * Used to get input MIDI event (from MIDI keyboard for example)
 */
class MidiIn {

    public:
        // 1 because it's the channel that work by default on my computer(tm)
        // eventually it will be replaced by a more rational default one
        // (read from user settings etc.)
        MidiIn(unsigned _chosenPort = 1);

        /**
         * Get the next MIDI in the buffer, return a "NullEvent" instance
         * if buffer is empty or input message not 
         */
        MidiEvent read();

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
