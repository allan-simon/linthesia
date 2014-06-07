#ifndef LINTHESIA_ABSTRACT_SCREEN_H
#define LINTHESIA_ABSTRACT_SCREEN_H

#include <SFML/Graphics.hpp>

#include "screen_index.h"

namespace linthesia {

class AbstractScreen {

    public:
        virtual ScreenIndex run(sf::RenderWindow &app) = 0;
        virtual ~AbstractScreen(){}

};

}
#endif
