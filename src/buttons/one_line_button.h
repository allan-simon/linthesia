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
         * Load the texture once for all
         */
        static void init();

        /**
         * Get the size of the common texture used for all
         * instance of this class, useful so that we don't need
         * to have an instance ready to prepare layout etc.
         */
        static const sf::Vector2u getSize();

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
         * static so the texture is load only once, and available
         * for all application lifetime
         */
        static sf::Texture texture;

        /**
         *
         */
        static sf::Font font;



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
        sf::Text label;
};

}

#endif

