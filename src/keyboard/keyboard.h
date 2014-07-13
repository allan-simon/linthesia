#ifndef LINTHESIA_KEYBOARD_H
#define LINTHESIA_KEYBOARD_H

#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "black_key.h"
#include "white_key.h"

namespace linthesia {

class Keyboard : public sf::Drawable , public sf::Transformable {

    public:
        Keyboard();


        void keyPressed(
            const unsigned noteNumber,
            const sf::Color &color = sf::Color::Blue
        );

        void keyReleased(
            const unsigned noteNumber
        );

        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

        const static unsigned NBR_WHITE_KEYS = 35;

        /**
         * get the x position of a black note given its midi note number
         */
        static unsigned xPositionBlackNote(const unsigned noteNumber);

        /**
         * Take a MIDI note number and determine if it's played by a black
         * key on keybaord
         */
        static bool isBlackKey(const unsigned noteNumber);

        /**
         * get the key's number on keyboard index from MIDI note number
         * Note: in order to know if it's a black or white key you need
         * to call isBlackKey first
         * Note: you need to call isOutOfKeyboard first to be sure its
         * a note playable by current keyboard
         */
        static unsigned noteToKeyboardIndex(const unsigned noteNumber);

        /**
         * If the key is not playable by current keyboard
         * (i.e is not playable by player and anyway will
         * create a out of bound exception)
         */
        static bool isOutOfKeyboard(const unsigned noteNumber);

    private:

        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;

        /**
         * get the note index in one octave (0 for Do/C, 6 for Si/B)
         * in order to know if white note index or black note index you need
         * to use "isBlackKey" method
         */
        static unsigned noteToOctaveIndex(
            const unsigned baseNoteNumber
        );


        std::vector<WhiteKey> whiteKeys;
        std::vector<BlackKey> blackKeys;

};


}

#endif

