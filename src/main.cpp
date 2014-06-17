#include <string>
#include <iostream>

#include <SFML/Window.hpp>

#include "libmidi/midi.h"
#include "libmidi/midi_util.h"

#include "screens/screens.h"

//TODO should not be needed
#include "buttons/short_one_line_button.h"
#include "buttons/long_one_line_button.h"

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

    auto screens = linthesia::init_game_screens();
    auto currentScreen = linthesia::START_APPLICATION;

    while (currentScreen != linthesia::STOP_APPLICATION) {
        currentScreen = screens[currentScreen]->run(application);
    }

    return 0;
}


