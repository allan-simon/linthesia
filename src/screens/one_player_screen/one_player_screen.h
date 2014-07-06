#ifndef LINTHESIA_ONE_PLAYER_SCREEN_H
#define LINTHESIA_ONE_PLAYER_SCREEN_H

#include "libmidi/midi.h"

#include "screens/abstract_screen.h"
#include "screens/screens.h"
#include "keyboard/keyboard.h"


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
        Keyboard keyboard;

        /**
         * Play the part of the song that should be played during that
         * delta time
         */
        void playSong(
            linthesia::Context &context,
            const sf::Time& delta
        );

        /**
         * Update the keyboard on screen according to input notes
         */
        void updateKeyboard(
            const MidiEventList& inputNotes,
            linthesia::Context &context
        );
};


}
#endif

