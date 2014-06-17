#ifndef LINTHESIA_ONE_LINE_BUTTON_H
#define LINTHESIA_ONE_LINE_BUTTON_H 

#include "abstract_button.h"

namespace linthesia {


/**
 * represent a generic one line button
 * with simply a texture and some text over it
 * it's not made to be used directly
 */
class OneLineButton : public AbstractButton {

    public :

        /** 
         *
         */
        OneLineButton();


        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

    protected:

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
        void setHovered();
        void setUnhovered();


        /**
         *
         */
        sf::Sprite sprite;
        /**
         *
         */
        sf::Text label;
};

}

#endif

