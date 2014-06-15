#ifndef LINTHESIA_ABSTRACT_SCREEN_H
#define LINTHESIA_ABSTRACT_SCREEN_H

#include <SFML/Graphics.hpp>

#include "screen_index.h"

namespace linthesia {

class AbstractScreen {

    public:
        virtual ScreenIndex run(sf::RenderWindow &app) = 0;
        virtual ~AbstractScreen(){}

    protected:
        /**
         * value used to have a consistent padding accross screen
         * and avoid buttons etc. to stick to the edges of the window
         * is not "pretty"
         */
        const unsigned PADDING = 10;
};

}
#endif
