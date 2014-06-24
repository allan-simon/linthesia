#include <iostream>
#include <vector>
#include "libmidi/midi_event.h"

#include "midi_out.h"

namespace linthesia {

MidiOut::MidiOut (unsigned _chosenPort):
    chosenPort(_chosenPort)
{
}
/**
 *
 */
void MidiOut::write(const MidiEvent &out) {
    std::vector<unsigned char>message(3);

    message[0] = out.get_status_code();
    message[1] = static_cast<unsigned char>(out.get_note_number());
    message[2] = static_cast<unsigned char>(out.get_note_velocity());
    rtMidiOut.sendMessage(&message);
}

/**
 *
 */
void MidiOut::open() {
    rtMidiOut.openPort(chosenPort);
}

/**
 *
 */
void MidiOut::close() {
    rtMidiOut.closePort();
}

/**
 *
 */
void MidiOut::switchNextPort() {
    chosenPort = (chosenPort + 1) % rtMidiOut.getPortCount();
}

/**
 *
 */
void MidiOut::switchPrevPort() {
    // we are using unsigned so we can't just do chosenPort - 1 % PORT_COUNT
    const unsigned PORT_COUNT = rtMidiOut.getPortCount();
    chosenPort = (chosenPort + PORT_COUNT - 1) % PORT_COUNT;
}

/**
 *
 */
std::string MidiOut::getCurrentOutputName() {
    return rtMidiOut.getPortName(chosenPort);
}


}
