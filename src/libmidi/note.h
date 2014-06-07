// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Copyright (c) 2014 Allan Simon <allan.simon@supinfo.com>
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#ifndef LINTHESIA_MIDI_NOTE_H
#define LINTHESIA_MIDI_NOTE_H

#include <cstdint>
#include <set>
#include "midi_types.h"

// Range of all 128 MIDI notes possible
typedef unsigned int NoteId;

// Arbitrary value outside the usual range
const static NoteId INVALID_NOTE_ID = 2048;

enum NoteState {
   AutoPlayed,
   UserPlayable,
   UserHit,
   UserMissed
};

template <class T>
struct GenericNote  {

    bool operator()(
        const GenericNote<T> &lhs,
        const GenericNote<T> &rhs
    ) const {
        if (lhs.start < rhs.start) return true;
        if (lhs.start > rhs.start) return false;

        if (lhs.end < rhs.end) return true;
        if (lhs.end > rhs.end) return false;

        if (lhs.noteId < rhs.noteId) return true;
        if (lhs.noteId > rhs.noteId) return false;

        if (lhs.trackId < rhs.trackId) return true;
        if (lhs.trackId > rhs.trackId) return false;

        return false;
    }

    T start;
    T end;

    NoteId noteId;
    size_t trackId;

    // We have to drag a little extra info around so we can
    // play the user's input correctly
    unsigned char channel;
    int velocity;

    // is the note an autplay one ? one the user is supposed to soon play etc.
    NoteState state;

};

// Note keeps the internal pulses found in the MIDI file which are
// independent of tempo or playback speed.
typedef GenericNote<uint32_t> Note;
// TranslatedNote contains the exact (translated) microsecond that notes
// start and stop on based on a given playback speed, after dereferencing
// tempo changes.
typedef GenericNote<microseconds_t> TranslatedNote;

typedef std::set<Note, Note> NoteSet;
typedef std::set<TranslatedNote, TranslatedNote> TranslatedNoteSet;

#endif

