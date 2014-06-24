#ifndef LINTHESIA_CONTEXT_H
#define LINTHESIA_CONTEXT_H

#include <SFML/System.hpp>
#include "libmidi/midi.h"
#include "midi_io/midi_out.h"
#include "midi_io/midi_in.h"

namespace linthesia {

class Context {
    
    public:
        Context();
        ~Context();

        bool openMidiFile(const std::string& filename);

        MidiEventListWithTrackId update(const sf::Int64 deltaMicroseconds);
        std::string getFilename() const;

        linthesia::MidiOut midiOut;
        linthesia::MidiIn midiIn;
    private:
        Midi* midi;
        std::string filename;

};


} // end namespace linthesia
#endif

