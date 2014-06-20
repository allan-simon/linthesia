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
        MidiOut();
        /**
         * Write the given midi event on the output
         * (for a standard note midi event it will play it)
         */
        void write(const MidiEvent &out);

        /**
         * Change the output port use when writing midi event
         */
        void set_chosen_port(unsigned newChosenport);

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

        // 1 because it's the channel that work by default on my computer(tm)
        // eventually it will be replaced by a more rational default one
        // (read from user settings etc.)
        unsigned chosenPort = 1;
};


} //
#endif
