#ifndef LINTHESIA_LONG_ONE_LINE_BUTTON_H
#define LINTHESIA_LONG_ONE_LINE_BUTTON_H 

#include "one_line_button.h"

namespace linthesia {


/**
 * represent a long button with some text hover it
 */
class LongOneLineButton : public OneLineButton {

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
        LongOneLineButton(
            const std::string& text
        );


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


};

}

#endif

