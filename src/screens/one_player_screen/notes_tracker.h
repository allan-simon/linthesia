#ifndef LINTHESIA_NOTES_TRACKER_H
#define LINTHESIA_NOTES_TRACKER_H

#include <map>
#include <list>
#include <vector>

#include "libmidi/midi.h"
#include "libmidi/note.h"
#include "context/context.h"

namespace linthesia {

const unsigned TOO_EARLY = 1000000;
const unsigned TOO_LATE = 1000000;

const int NO_SUCH_NOTE_IN_SONG = -1;
const int NO_MORE_SUCH_NOTE_IN_SONG = -2;
const int NOTE_HIT_TOO_EARLY = -3;

/**
 * keep track of which notes still need to be played
 * TODO: add support to keep track of which note has been played and how
 *       so that we can have some "playback" and also manage going
 *       back in time
 */
class NotesTracker {

    public:
        /**
         * use the context to fill the note tracker with notes
         */
        void fill(const Context &context);

        /**
         * check if the note `noteId` touched at the time `hitTime`
         * corresponds to a note that was supposed to be played, if so
         * remove it from the notes waiting to be played and return
         * the delta time to know how precise was the hit
         * return negative number in case the user failed miserably
         */
        int hitNote(NoteId noteid, microseconds_t hitTime);

        /**
         * walk through the map of notes to check if some notes
         * have been missed, and return the list of missed notes
         */
        std::vector<TranslatedNote> checkMissedNotes(
            microseconds_t currentTime
        );

    private:

        /**
         * We index the notes by their note id (do/re/mi etc.)
         * to be played
         */
        std::map<
            NoteId,
            std::list<TranslatedNote>
        > notesByNoteId;
};

}
#endif

