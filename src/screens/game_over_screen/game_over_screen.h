#ifndef LINTHESIA_GAME_OVER_SCREEN_H
#define LINTHESIA_GAME_OVER_SCREEN_H

#include "screens/abstract_screen.h"
#include "screens/screens.h"

namespace linthesia {

/**
 * Represent the screen at the end of a song showing
 * the final score and statistics
 * @class GameOverScreen
 */
class GameOverScreen : public AbstractScreen {

    public:
        const static ScreenIndex INDEX;
        ScreenIndex run(sf::RenderWindow &app); 

};


}
#endif

