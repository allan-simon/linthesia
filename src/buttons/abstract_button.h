#ifndef LINTHESIA_ABSTRACT_BUTTON_H
#define LINTHESIA_ABSTRACT_BUTTON_H

#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace linthesia {

enum ButtonStates {
    NORMAL,
    HOVERED,
    CLICKED,
    DISABLED
};

/**
 * represent a button with some text hover it
 */
class AbstractButton : public sf::Drawable , public sf::Transformable {

    public :

        /** 
         * @param text text to put hover the button
         */
        AbstractButton(
            const std::string& text
        );

        /**
         * Check if the button state needs to be changed
         * (set as hovered, clicked etc.) and change it
         * We return true if we are in the state in which
         * its action needs to be triggered (typically when
         * we click on it)
         * TODO: find a better name
         */
        bool actionTriggered(const sf::Window &app);

        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

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
        bool containsPoint(sf::Vector2i point) const;

        /**
         *
         */
        void setHovered();
        void setUnhovered();

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
        sf::Text label;

        /**
         *
         */
        sf::Font font;

        /**
         *
         */
        ButtonStates currentState;
};

}

#endif

