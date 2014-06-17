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
         * Check if the button state needs to be changed
         * (set as hovered, clicked etc.) and change it
         * We return true if we are in the state in which
         * its action needs to be triggered (typically when
         * we click on it)
         * TODO: find a better name
         */
        virtual bool actionTriggered(const sf::Window &app);

        /**
         *
         */
        virtual sf::FloatRect getGlobalBounds() const = 0;

    protected:

        /**
         *
         */
        virtual void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const = 0;

        /**
         *
         */
        virtual void setHovered() = 0;
        virtual void setUnhovered() = 0;

        /**
         *
         */
        bool containsPoint(sf::Vector2i point) const;

        ButtonStates currentState;
};

}

#endif

