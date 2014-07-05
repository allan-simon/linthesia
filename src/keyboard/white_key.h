#ifndef LINTHESIA_WHITE_KEY_H
#define LINTHESIA_WHITE_KEY_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace linthesia {

/**
 * Represent a white key on the keyboard, and all the transformation
 * we can apply to it (played/add note name/etc.)
 */
class WhiteKey : public sf::Drawable , public sf::Transformable {

   public:
        WhiteKey();

        /**
         * Action to execute when the key is pressed
         * Note: we give a color directly rather
         * than something more abstract like a "trackid" because otherwise
         * we would have needed something to map trackId to a color, which
         * would have been hard to manage when the user will be able to choose
         * himself the color of each track
         */
        void pressed(sf::Color color);

        /**
         * Set back key to normal state when key is released:
         */
        void released();

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
        sf::RectangleShape whiteBody;
        sf::RectangleShape blackLine;

};


}
#endif

