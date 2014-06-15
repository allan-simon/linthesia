#ifndef LINTHESIA_MAIN_SCREEN_H
#define LINTHESIA_MAIN_SCREEN_H

#include "screens/abstract_screen.h"
#include "screens/screens.h"

#include "buttons/abstract_button.h"

namespace linthesia {

class MainScreen : public AbstractScreen {

    public:
        MainScreen();
        const static ScreenIndex INDEX;
        ScreenIndex run(sf::RenderWindow &app);

    private:
        void setExitButtonPosition(sf::RenderWindow &app);

        linthesia::AbstractButton exitButton;
};


}
#endif

