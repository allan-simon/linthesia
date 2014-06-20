#include <iostream>
#include <vector>
#include "libmidi/midi_event.h"

#include "midi_out.h"

namespace linthesia {

MidiOut::MidiOut () {
}
/**
 *
 */
void MidiOut::write(const MidiEvent &out) {
    std::vector<unsigned char>message(3);

    message[0] = out.get_status_code();
    message[1] = static_cast<unsigned char>(out.get_note_number());
    message[2] = 90;//static_cast<unsigned char>(out.get_note_velocity());
    std::cout << static_cast<int>(message[0]) << std::endl;
    std::cout << out.get_note_number() << std::endl;
    std::cout << out.get_note_velocity() << std::endl;
    std::cout <<std::endl;
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
void MidiOut::set_chosen_port(unsigned newChosenport) {
    chosenPort = newChosenport;
}


}
