#include <iostream>
#include <vector>
#include <unistd.h>
#include "libmidi/midi_event.h"
#include "libmidi/midi_util.h"

#include "midi_in.h"

static void mycallback( double deltatime, std::vector< unsigned char > *message, void *userData )
{
unsigned int nBytes = message->size();
for ( unsigned int i=0; i<nBytes; i++ )
std::cout << "Byte " << i << " = " << (int)message->at(i) << ", " << std::endl;
if ( nBytes > 0 )
std::cout << "stamp = " << deltatime << std::endl;
}


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
void MidiIn::open() {
    rtMidiIn.openPort(chosenPort);
    //rtMidiIn.setCallback(&mycallback);
    //std::cout << " we open port " << chosenPort << std::endl;
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
