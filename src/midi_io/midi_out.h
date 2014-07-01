#ifndef LINTHESIA_MIDI_OUT_H
#define LINTHESIA_MIDI_OUT_H

#include <RtMidi.h>

class MidiEvent;

namespace linthesia {

/**
 * Used to output midi event
 */
class MidiOut {

    public:
        MidiOut(unsigned _chosenPort = 0);
        /**
         * Write the given midi event on the output
         * (for a standard note midi event it will play it)
         */
        void write(const MidiEvent &out);

        /**
         * Change the output port use when writing midi event
         */
        void switchNextPort();
        void switchPrevPort();

        /**
         *
         */
        std::string getCurrentOutputName();

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
        RtMidiOut  rtMidiOut;

        unsigned chosenPort;
};


} //
#endif
