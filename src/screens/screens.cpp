#include "screens.h"
#include "screens/main_screen/main_screen.h"
#include "screens/select_track_screen//select_track_screen.h"

//TODO: revert the dependency by providing a static function
//for screens to register themselves

namespace linthesia {

Screens init_game_screens () {

    Screens screens;

    screens[MainScreen::INDEX] =std::unique_ptr<MainScreen> (new MainScreen);
    screens[SelectTrackScreen::INDEX] = std::unique_ptr<SelectTrackScreen> (new SelectTrackScreen);

    return screens;
}

}

