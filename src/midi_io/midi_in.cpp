#include <iostream>
#include <vector>
#include <unistd.h>
#include "libmidi/midi_event.h"
#include "libmidi/midi_util.h"

#include "midi_in.h"

namespace linthesia {

MidiIn::MidiIn (unsigned _chosenPort):
    chosenPort(_chosenPort)
{
    std::cout << "constructor called" << std::endl;
}
/**
 *
 */
MidiEvent MidiIn::read() {

    std::vector<unsigned char>message;
    rtMidiIn.getMessage(&message);

    if (message.empty()) {
       return MidiEvent::NullEvent();
    }
    std::cout << "we read something" << std::endl;

    MidiEventSimple simple;
    simple.status = message[0];
    simple.byte1 = message[1];
    simple.byte2 = message[2];

    return MidiEvent::build(simple);
}

/**
 *
 */
MidiEventList MidiIn::readAllNotes() {
    MidiEventList allNotes;
    //TODO: should be possible to transform it into
    // a do while
    MidiEvent event = read();
    while(
        event.get_type() == MidiEventType_NoteOff ||
        event.get_type() == MidiEventType_NoteOn
    ) {
        allNotes.push_back(event);
        event = read();
    }
    return allNotes;
}

/**
 *
 */
void MidiIn::open() {
    rtMidiIn.openPort(chosenPort);
}

/**
 *
 */
void MidiIn::close() {
    std::cout << " we close port " << chosenPort << std::endl;
    rtMidiIn.closePort();
}

/**
 *
 */
void MidiIn::switchNextPort() {
    chosenPort = (chosenPort + 1) % rtMidiIn.getPortCount();
}

/**
 *
 */
void MidiIn::switchPrevPort() {
    // we are using unsigned so we can't just do chosenPort - 1 % PORT_COUNT
    const unsigned PORT_COUNT = rtMidiIn.getPortCount();
    chosenPort = (chosenPort + PORT_COUNT - 1) % PORT_COUNT;
}

/**
 *
 */
std::string MidiIn::getCurrentPortName() {
    return rtMidiIn.getPortName(chosenPort);
}


}
