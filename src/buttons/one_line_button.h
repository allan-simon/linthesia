#ifndef LINTHESIA_ONE_LINE_BUTTON_H
#define LINTHESIA_ONE_LINE_BUTTON_H 

#include "abstract_button.h"

namespace linthesia {


/**
 * represent a button with some text hover it
 */
class OneLineButton : public AbstractButton {

    public :

        /** 
         * @param text text to put hover the button
         */
        OneLineButton(
            const std::string& text
        );

        /**
         * @see AbstractButton::actionTriggered
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

};

}

#endif

