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

    private:

        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;


        /**
         *
         */
        static bool isBlackKey(const unsigned noteNumber);

        /**
         *
         */
        static unsigned noteToIndex(
            const unsigned baseNoteNumber
        );

        /**
         * If the key is not playable by current keyboard
         * (i.e is not playable by player and anyway will
         * create a out of bound exception)
         */
        static bool isOutOfKeyboard(const unsigned noteNumber);

        std::vector<WhiteKey> whiteKeys;
        std::vector<BlackKey> blackKeys;

};


}

#endif

