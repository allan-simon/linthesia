#ifndef LINTHESIA_ABSTRACT_BUTTON_H
#define LINTHESIA_ABSTRACT_BUTTON_H

#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace linthesia {
class AbstractButton : public sf::Drawable , public sf::Transformable {

    public :
        /** 
         *
         */
        AbstractButton(
            const std::string &text
        );

            
        /**
         * TODO maybe replace directly by a "isClicked" ?
         */
        bool containsPoint(sf::Vector2i point) const;

    private:

        /**
         *
         */
        virtual void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;

        /**
         *
         */
        sf::Sprite sprite;

        /**
         *
         */
        sf::Texture texture;

        /**
         *
         */
        sf::Text text;
};

}

#endif

