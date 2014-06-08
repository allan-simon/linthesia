#include "screens.h"
#include "screens/main_screen/main_screen.h"
#include "screens/select_track_screen/select_track_screen.h"
#include "screens/file_select_screen/file_select_screen.h"
#include "screens/game_over_screen/game_over_screen.h"
#include "screens/one_player_screen/one_player_screen.h"

//TODO: revert the dependency by providing a static function
//for screens to register themselves

namespace linthesia {

Screens init_game_screens () {

    Screens screens;

    screens[MainScreen::INDEX] =std::unique_ptr<MainScreen> (new MainScreen);
    screens[SelectTrackScreen::INDEX] = std::unique_ptr<SelectTrackScreen> (new SelectTrackScreen);
    screens[FileSelectScreen::INDEX] = std::unique_ptr<FileSelectScreen> (new FileSelectScreen);
    screens[OnePlayerScreen::INDEX] = std::unique_ptr<OnePlayerScreen> (new OnePlayerScreen);
    screens[GameOverScreen::INDEX] = std::unique_ptr<GameOverScreen> (new GameOverScreen);

    return screens;
}

}

