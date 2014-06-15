#ifndef LINTHESIA_FILE_SELECT_SCREEN_H
#define LINTHESIA_FILE_SELECT_SCREEN_H

#include "screens/abstract_screen.h"
#include "screens/screens.h"

#include "buttons/one_line_button.h"

namespace linthesia {

class FileSelectScreen : public AbstractScreen {

    public:
        const static ScreenIndex INDEX;
        ScreenIndex run(sf::RenderWindow &app); 

    private:
        std::vector<OneLineButton> fileButtons;
};


}
#endif

