// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#include "midi_event.h"
#include "midi_util.h"
#include "note.h"

using namespace std;

/**
 *
 */
MidiEvent MidiEvent::read_from_stream(
    istream &stream,
    unsigned char lastStatus,
    bool containsDeltaPulses
) {
    MidiEvent event;

    if (containsDeltaPulses) {
        event.deltaPulses = parse_variable_length(stream);
    } else {
        event.deltaPulses = 0;
    }

    // MIDI uses a compression mechanism called "running status".
    // Anytime you read a status byte that doesn't have the highest-
    // order bit set, what you actually read is the 1st data byte
    // of a message with the status of the previous message.
    event.status = static_cast<unsigned char>(stream.peek());

    if ((event.status & 0x80) == 0) {
        event.status = lastStatus;
    } else {
        // It was a status byte after all, just read past it
        // in the stream
        stream.read(reinterpret_cast<char*>(&event.status), sizeof(unsigned char));
    }

    switch (event.get_type()) {
        case MidiEventType_Meta:
            event.read_meta(stream);
            break;

        case MidiEventType_SysEx:
            event.read_sys_ex(stream);
            break;

        default:
            event.read_standard(stream);
            break;
    }

    return event;
}

/**
 *
 */
MidiEvent MidiEvent::build(const MidiEventSimple &simple) {
    MidiEvent event;

    event.deltaPulses = 0;
    event.status = simple.status;
    event.data1 = simple.byte1;
    event.data2 = simple.byte2;

    if (event.get_type() == MidiEventType_Meta) {
        throw MidiError(MidiError_MetaEventOnInput);
    }

    return event;
}

/**
 *
 */
MidiEvent MidiEvent::NullEvent() {
    MidiEvent event;

    event.status = 0xFF;
    event.metaType = MidiMetaEvent_Proprietary;
    event.deltaPulses = 0;

    return event;
}

/**
 *
 */
void MidiEvent::read_meta(istream &stream) {
    stream.read(reinterpret_cast<char*>(&metaType), sizeof(unsigned char));
    uint32_t metaLength = parse_variable_length(stream);

    char *buffer = new char[metaLength + 1];
    buffer[metaLength] = 0;

    stream.read(buffer, metaLength);
    if (stream.fail()) {
        delete[] buffer;
        throw MidiError(MidiError_EventTooShort);
    }

    switch (metaType) {
        case MidiMetaEvent_Text:
        case MidiMetaEvent_Copyright:
        case MidiMetaEvent_TrackName:
        case MidiMetaEvent_Instrument:
        case MidiMetaEvent_Lyric:
        case MidiMetaEvent_Marker:
        case MidiMetaEvent_Cue:
        case MidiMetaEvent_PatchName:
        case MidiMetaEvent_DeviceName:
            text = string(buffer, metaLength);
            break;

        case MidiMetaEvent_TempoChange: {
            if (metaLength < 3) {
                throw MidiError(MidiError_EventTooShort);
            }

            // We have to convert to unsigned char first for some reason or the
            // conversion gets all wacky and tries to look at more than just its
            // one byte at a time.
            unsigned int b0 = static_cast<unsigned char>(buffer[0]);
            unsigned int b1 = static_cast<unsigned char>(buffer[1]);
            unsigned int b2 = static_cast<unsigned char>(buffer[2]);
            tempoMsPerQuaterNote = (b0 << 16) + (b1 << 8) + b2;

            break;
        }

        case MidiMetaEvent_SequenceNumber:
        case MidiMetaEvent_EndOfTrack:
        case MidiMetaEvent_SMPTEOffset:
        case MidiMetaEvent_TimeSignature:
        case MidiMetaEvent_KeySignature:
        case MidiMetaEvent_Proprietary:
        case MidiMetaEvent_ChannelPrefix:
        case MidiMetaEvent_MidiPort:
            // NOTE: We would have to keep all of this around if we
            // wanted to reproduce 1:1 MIDIs between file Save/Load
            break;

        default:
            delete[] buffer;
            throw MidiError(MidiError_UnknownMetaEventType);
    }

    delete[] buffer;
}

/**
 *
 */
void MidiEvent::read_sys_ex(istream &stream) {
    // NOTE: We would have to keep SysEx events around if we
    // wanted to reproduce 1:1 MIDIs between file Save/Load
    uint32_t sysExLength = parse_variable_length(stream);

    // Discard
    char *buffer = new char[sysExLength];
    stream.read(buffer, sysExLength);
    delete[] buffer;
}

/**
 *
 */
void MidiEvent::read_standard(istream &stream) {
    switch (get_type()) {
        case MidiEventType_NoteOff:
        case MidiEventType_NoteOn:
        case MidiEventType_Aftertouch:
        case MidiEventType_Controller:
        case MidiEventType_PitchWheel:
            stream.read(reinterpret_cast<char*>(&data1), sizeof(unsigned char));
            stream.read(reinterpret_cast<char*>(&data2), sizeof(unsigned char));
            break;

        case MidiEventType_ProgramChange:
        case MidiEventType_ChannelPressure:
            stream.read(reinterpret_cast<char*>(&data1), sizeof(unsigned char));
            data2 = 0;
            break;

        default:
            throw MidiError(MidiError_UnknownEventType);
    }
}

/**
 *
 */
bool MidiEvent::get_simple_event(MidiEventSimple *simple) const {
    MidiEventType type = get_type();
    if (
        type == MidiEventType_Meta ||
        type == MidiEventType_SysEx ||
        type == MidiEventType_Unknown
    ) {
        return false;
    }

    simple->status = status;
    simple->byte1 = data1;
    simple->byte2 = data2;

    return true;
}

/**
 *
 */
bool MidiEvent::is_playable() const {
    return
        get_type() == MidiEventType_NoteOn ||
        get_type() == MidiEventType_NoteOff;
}

/**
 *
 */
bool MidiEvent::is_note_on() const {
    return get_type() == MidiEventType_NoteOn;
}

/**
 *
 */
bool MidiEvent::is_note_off() const {
    return get_type() == MidiEventType_NoteOff;
}

/**
 *
 */
MidiEventType MidiEvent::get_type() const {
    if (status >  0xEF && status < 0xFF) {
        return MidiEventType_SysEx;
    }

    if (status <  0x80) {
        return MidiEventType_Unknown;
    }

    if (status == 0xFF) {
        return MidiEventType_Meta;
    }

    // The 0x8_ through 0xE_ events contain channel numbers
    // in the lowest 4 bits
    unsigned char statusTop = status >> 4;

    switch (statusTop) {
        case 0x8:
            return MidiEventType_NoteOff;

        case 0x9:
            return MidiEventType_NoteOn;

        case 0xA:
            return MidiEventType_Aftertouch;

        case 0xB:
            return MidiEventType_Controller;

        case 0xC:
            return MidiEventType_ProgramChange;

        case 0xD:
            return MidiEventType_ChannelPressure;

        case 0xE:
            return MidiEventType_PitchWheel;

        default:
            return MidiEventType_Unknown;
    }
}

/**
 *
 */
MidiMetaEventType MidiEvent::get_meta_type() const {
    if (get_type() != MidiEventType_Meta) {
        return MidiMetaEvent_Unknown;
    }

    return static_cast<MidiMetaEventType>(metaType);
}

/**
 *
 */
bool MidiEvent::is_end() const {
    return (
        get_type() == MidiEventType_Meta &&
        get_meta_type() == MidiMetaEvent_EndOfTrack
    );
}

/**
 *
 */
unsigned char MidiEvent::get_channel() const {
    // The channel is held in the lower nibble of the status code
    return (status & 0x0F);
}

/**
 *
 */
void MidiEvent::set_channel(unsigned char channel) {
    if (channel > 15) {
        return;
    }

    // Clear out the old channel
    status = status & 0xF0;

    // Set the new channel
    status = status | channel;
}

/**
 *
 */
void MidiEvent::set_velocity(int velocity) {
    if (get_type() != MidiEventType_NoteOn) {
        return;
    }

    data2 = static_cast<unsigned char>(velocity);
}

/**
 *
 */
bool MidiEvent::has_text() const {
    if (get_type() != MidiEventType_Meta) {
        return false;
    }

    switch (metaType) {
        case MidiMetaEvent_Text:
        case MidiMetaEvent_Copyright:
        case MidiMetaEvent_TrackName:
        case MidiMetaEvent_Instrument:
        case MidiMetaEvent_Lyric:
        case MidiMetaEvent_Marker:
        case MidiMetaEvent_Cue:
        case MidiMetaEvent_PatchName:
        case MidiMetaEvent_DeviceName:
            return true;

        default:
            return false;
    }
}

/**
 *
 */
NoteId MidiEvent::get_note_number() const {
    if (
        get_type() != MidiEventType_NoteOn &&
        get_type() != MidiEventType_NoteOff
    ) {
        return 0;
    }

    return data1;
}

/**
 *
 */
void MidiEvent::shift_note(int shift_amount) {
    if (
        get_type() != MidiEventType_NoteOn &&
        get_type() != MidiEventType_NoteOff
    ) {
        return;
    }

    data1 = data1 + static_cast<unsigned char>(shift_amount);
}

/**
 *
 */
int MidiEvent::get_program_number() const {
    if (get_type() != MidiEventType_ProgramChange) {
        return 0;
    }

    return data1;
}

/**
 *
 */
string MidiEvent::get_note_name(unsigned int noteNumber) {
    // Music domain knowledge
    const static unsigned int NOTES_PER_OCTAVE = 12;
    const static string NOTE_BASES[NOTES_PER_OCTAVE] = {
        STRING("C"),  STRING("C#"), STRING("D"),
        STRING("D#"), STRING("E"),  STRING("F"),
        STRING("F#"), STRING("G"),  STRING("G#"),
        STRING("A"),  STRING("A#"), STRING("B")
    };

    unsigned int octave = (noteNumber / NOTES_PER_OCTAVE);
    const string noteBase = NOTE_BASES[noteNumber % NOTES_PER_OCTAVE];

    return STRING(noteBase << octave);
}

/**
 *
 */
int MidiEvent::get_note_velocity() const {
    if (get_type() == MidiEventType_NoteOff) {
        return 0;
    }

    if (get_type() != MidiEventType_NoteOn) {
        return -1;
    }

    return static_cast<int>(data2);
}

/**
 *
 */
string MidiEvent::get_text() const {
    if (!has_text()) {
        return "";
    }

    return text;
}

/**
 *
 */
uint32_t MidiEvent::get_temps_in_ms_per_quater_note() const {
    if (
        get_type() != MidiEventType_Meta ||
        get_meta_type() != MidiMetaEvent_TempoChange
    ) {
        throw MidiError(MidiError_RequestedTempoFromNonTempoEvent);
    }

    return tempoMsPerQuaterNote;
}

/**
 *
 */
bool MidiEvent::is_percussion_channel() const {
    // These are actually 10 and 16 in the MIDI standard.  However, MIDI
    // channels are 1-based facing the user.  They're stored 0-based.
    //XXX move that in midi_event.cpp
    const static int PERCUSSION_CHANNEL_1 = 9;
    const static int PERCUSSION_CHANNEL_2 = 15;

    return (
        get_channel() == PERCUSSION_CHANNEL_1 ||
        get_channel() == PERCUSSION_CHANNEL_2
    );

}
