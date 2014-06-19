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
        void write(const MidiEvent &out);
    
    private:
        RtMidiOut  rtMidiOut;

};


} //
#endif
