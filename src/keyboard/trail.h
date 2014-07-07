#ifndef LINTHESIA_KEYBOARD_TRAIL_H
#define LINTHESIA_KEYBOARD_TRAIL_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace linthesia {

/**
 * Represent the trail at the top
 */
class KeyboardTrail : public sf::Drawable , public sf::Transformable {
    public:
        KeyboardTrail(const int width);

        /**
         * get the size of keyboard trail
         */
        const sf::FloatRect getGlobalBounds() const;

    private:

        sf::Texture texture;  

        sf::Sprite sprite; 

        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;


};

}

#endif

