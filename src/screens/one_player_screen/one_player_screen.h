#ifndef LINTHESIA_ONE_PLAYER_SCREEN_H
#define LINTHESIA_ONE_PLAYER_SCREEN_H

#include "libmidi/midi.h"

#include "screens/abstract_screen.h"
#include "screens/screens.h"
#include "keyboard/keyboard.h"
#include "keyboard/trail.h"
#include "note_ground.h"
#include "notes_tracker.h"
#include "score_display.h"

namespace linthesia {

/**
 * Represent the screen for one player to play a song
 * @class OnePlayerScreen
 */
class OnePlayerScreen : public AbstractScreen {

    public:
        const static ScreenIndex INDEX;
        ScreenIndex run(
            sf::RenderWindow &app,
            Context &context
        );


    private:
        /**
         * Arbitrary height at the bottom that we will use latter to put score
         * play/pause buttons etc.
         */
        const unsigned SPACE_BUTTONS = 60;

        /**
         *
         */
        void setKeyboardPosition(const sf::RenderWindow &app);
        Keyboard keyboard;

        /**
         * set the "separator" (the "trail") at the top of the keyboard
         */
        void setKeyboardTrailPosition(
            const sf::RenderWindow &app
        );
        KeyboardTrail keyboardTrail;

        /**
         * Set the label displaying the number of notes hit
         * by category (good/excellent/missed etc.)
         */
        void setScoreDisplayPosition(
            const sf::RenderWindow &app
        );
        ScoreDisplay scoreDisplay;
        Score score;

        /**
         *
         */
        NoteGround noteGround;

        /**
         * Used by scrollNoteGround to keep track of rounding 'error'
         * (fix #55)
         */
        unsigned int nonScrolledMicroSec = 0;

        /**
         * speed at which the song is played
         * 1   : normal speed
         * > 1 : faster than normal
         * < 1 : slower than normal
         */
        float speedFactor = 1.0f;

        /**
         * Set the dimension and position of the notes ground view
         * that will be latter scrolled down as we play the song
         */
        void setNoteGroundView(
            const sf::RenderWindow &app
        );

        /**
         * Use speed (well invert of speed as it's microsec per pixel)
         * and delta time to scroll the noteGround accordingly
         */
        void scrollNoteGround(
            const unsigned microSecondPerPixel,
            const sf::Time& delta
        );
        sf::View noteGroundView;

        /**
         * TODO: should be moved in an other object
         * Play the part of the song that should be played during that
         * delta time
         */
        void playSong(
            linthesia::Context &context,
            const MidiEventListWithTrackId &events
        );

        /**
         * TODO should be moved in an other object
         */
        void playInputNotes(
            linthesia::Context &context,
            const MidiEventList& inputNotes
        );

        /**
         * Update the keyboard on screen according to input notes
         */
        void updateKeyboard(
            const MidiEventList& inputNotes,
            linthesia::Context &context
        );

        /**
         * check if the input notes correspond to notes that should be
         * played
         */
        void checkHitNotes(
            const MidiEventList& inputNotes,
            linthesia::Context &context
        );

        /**
         * Take the context (midi file/colors/ which tracks are played etc.)
         * to create the ground of notes
         */
        void initNoteGround(
            const Context &context
        );

        /**
         *
         */
        NotesTracker notesTracker;
};


}
#endif

