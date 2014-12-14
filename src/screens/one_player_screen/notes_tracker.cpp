#include "notes_tracker.h"

namespace linthesia {


/**
 *
 */
void NotesTracker::fill(const Context &context) {

    notesByNoteId.clear();

    for (auto note : context.getNotes()) {
        if (!context.tracksOptions.isPlayedByComputer(note.channel)) {
            notesByNoteId[note.noteId].push_back(note);
        }
    }
}

/**
 *
 */
std::vector<TranslatedNote> NotesTracker::checkMissedNotes(
    microseconds_t time
) {

    std::vector<TranslatedNote> missedNotes;

    for (auto& kv : notesByNoteId) {
        auto& noteQueue = kv.second;
        if (noteQueue.empty()) {
            continue;
        }
        unsigned start = noteQueue.front().start;
        if (
            // if the notes should have been played already
            start < time &&
            // for more than 1s second
            ((time - start) > TOO_LATE)
        ) {
            missedNotes.push_back(noteQueue.front());
            noteQueue.pop_front();
        }
    }

    return missedNotes;
}

/**
 *
 */
int NotesTracker::hitNote(
    NoteId noteId,
    microseconds_t hitTime
) {
    // if the user plays a note that is not present at all in the
    // song
    if (notesByNoteId.find(noteId) == notesByNoteId.end()) {
        return NO_SUCH_NOTE_IN_SONG;
    }
    // if the user plays a note that is no more present in the song
    if (notesByNoteId[noteId].empty()) {
        return NO_MORE_SUCH_NOTE_IN_SONG;
    }
   
    auto noteStart = notesByNoteId[noteId].front().start;
    auto delta = (noteStart > hitTime) ?
        noteStart - hitTime :
        hitTime - noteStart
    ;

    // if the note is played too early we don't count it has played
    if (delta > TOO_EARLY) {
        return NOTE_HIT_TOO_EARLY;
    }

    // TODO idea: instead of just "pop" and throwing away note
    // maybe put them in a stack of "played" note ?

    // TODO: we need actually to pop only when the key is released
    // right now we take care only about the 'start' not the end
    notesByNoteId[noteId].pop_front();
    
    return delta;
}

}
