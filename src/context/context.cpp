#include <iostream>
#include "libmidi/midi_util.h"

#include "context.h"
namespace linthesia {


/**
 *
 */
Context::Context(): midi(nullptr) { }

/**
 *
 */
Context::~Context() {
    delete midi;
}

/**
 *
 */
bool Context::openMidiFile(const std::string& _filename) {

    try {
        // in case of multiple open, we delete the previous midi file
        delete midi;
        midi = new Midi(Midi::read_from_file(_filename));
        filename = filename;
    } catch (const MidiError& exception) {
        std::cerr << exception.get_error_description() << std::endl;
        std::cerr << "while opening " << _filename << std::endl;

        return false;
    }

    return true;
}

/**
 *
 */
std::string Context::getFilename() const {
    return filename;
}

} // end namespace linthesia

