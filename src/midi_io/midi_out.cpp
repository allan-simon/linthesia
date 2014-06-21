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
    message[2] = 90;//static_cast<unsigned char>(out.get_note_velocity());
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
    chosenPort = (chosenPort - 1) % rtMidiOut.getPortCount();
}

/**
 *
 */
std::string MidiOut::getCurrentOutputName() {
    return rtMidiOut.getPortName(chosenPort);
}


}
