#ifndef LINTHESIA_CONTEXT_H
#define LINTHESIA_CONTEXT_H

#include "libmidi/midi.h"

namespace linthesia {

class Context {
    
    public:
        Context();
        ~Context();

        bool openMidiFile(const std::string& filename);

        std::string getFilename() const;

    private:
        Midi* midi;
        std::string filename;
};


} // end namespace linthesia
#endif

