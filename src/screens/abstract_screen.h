#ifndef LINTHESIA_ABSTRACT_SCREEN_H
#define LINTHESIA_ABSTRACT_SCREEN_H

#include <SFML/Window.hpp>

#include "screen_index.h"

namespace linthesia {

class AbstractScreen {

    public:
        virtual ScreenIndex run(sf::Window &app) = 0;
        virtual ~AbstractScreen(){}

};

}
#endif
