#include <iostream>

#include "file_select_screen.h"

namespace linthesia {

/**
 *
 */
const ScreenIndex FileSelectScreen::INDEX = "select_tracks_creen";

/**
 *
 */
const static auto BACKGROUND_COLOR = sf::Color(64, 64, 64);

/**
 *
 */
ScreenIndex FileSelectScreen::run(sf::RenderWindow &app) {
    sf::Event event;

    std::cout << INDEX << std::endl;

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return START_APPLICATION;
            }
        }
        app.clear(BACKGROUND_COLOR);
        app.display();
    }
}

} // end namespace linthesia

