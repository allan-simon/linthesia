#include <string>
#include <iostream>

#include <SFML/Window.hpp>
#include "screens/screens.h"
#include "context/context.h"

//TODO should not be needed
#include "buttons/short_one_line_button.h"
#include "buttons/long_one_line_button.h"
#include "screens/select_track_screen/track_box.h"
#include "icons/icon.h"

using namespace std;


int main() {

    sf::RenderWindow application(
        sf::VideoMode(800, 600),
        "Linthesia"
    );
    application.setVerticalSyncEnabled(true);

    //TODO should not be needed here
    linthesia::ShortOneLineButton::init();
    linthesia::LongOneLineButton::init();
    linthesia::TrackBox::init();
    linthesia::Icon::init();

    auto screens = linthesia::init_game_screens();
    auto currentScreen = linthesia::START_APPLICATION;

    auto context = linthesia::Context();

    while (currentScreen != linthesia::STOP_APPLICATION) {
        currentScreen = screens[currentScreen]->run(
            application,
            context
        );
    }
    application.close();

    return 0;
}


