#ifndef LINTHESIA_ONE_PLAYER_SCREEN_H
#define LINTHESIA_ONE_PLAYER_SCREEN_H

#include "screens/abstract_screen.h"
#include "screens/screens.h"

namespace linthesia {

/**
 * Represent the screen for one player to play a song
 * @class OnePlayerScreen
 */
class OnePlayerScreen : public AbstractScreen {

    public:
        const static ScreenIndex INDEX;
        ScreenIndex run(sf::RenderWindow &app); 

};


}
#endif

