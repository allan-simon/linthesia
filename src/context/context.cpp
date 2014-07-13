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
        filename = _filename;
    } catch (const MidiError& exception) {
        // make sure that we have a nullptr
        midi = nullptr;
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

/**
 *
 */
MidiEventListWithTrackId Context::update(const sf::Int64 deltaMicroseconds) {
    if (midi == nullptr) {
        return MidiEventListWithTrackId();
    }
    return midi->update(static_cast<microseconds_t>(deltaMicroseconds));
}

/**
 *
 */
const TranslatedNoteSet &Context::getNotes() const {
    return midi->get_notes();
}



/**
 *
 */
sf::Color Context::getChannelColor(const unsigned channel) const {

    const sf::Uint8 primary = 200;
    const sf::Uint8 secondary = 50;
    switch(channel) {
        case 1: return sf::Color(secondary, primary, secondary); // ~green
        case 2: return sf::Color(secondary, primary, primary); // ~cyan
        case 3: return sf::Color(primary, primary, secondary); // ~yellow
        case 4: return sf::Color(primary, secondary, primary); // ~magenta
        case 5: return sf::Color(secondary, secondary, primary); // ~blue
        default: return sf::Color(primary, secondary, secondary); // ~red
    }
}

/**
 *
 */
const MidiTrackList& Context::getTracks() const {
    // TODO: throw exception if midi is null
    return midi->get_tracks();
}

} // end namespace linthesia

