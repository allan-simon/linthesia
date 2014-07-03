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

