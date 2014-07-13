#ifndef LINTHESIA_BLACK_KEY_H
#define LINTHESIA_BLACK_KEY_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace linthesia {

/**
 * Represent a black key on the keyboard, and all the transformation
 * we can apply to it (played/add note name/etc.)
 */
class BlackKey : public sf::Drawable , public sf::Transformable {

   public:
        BlackKey();

        const static int BLACK_KEY_WIDTH = 12;
        const static int BLACK_KEY_HEIGHT = 67;



        /**
         * Action to execute when the key is pressed
         * @see WhiteKey::pressed for more details
         */
        void pressed(const sf::Color &color);

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
        sf::RectangleShape blackBody;

};


}
#endif


