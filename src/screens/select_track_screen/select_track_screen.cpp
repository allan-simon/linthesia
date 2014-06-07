#include <iostream>

#include "select_track_screen.h"

namespace linthesia {

/**
 *
 */
const ScreenIndex SelectTrackScreen::INDEX = "select_tracks_creen";

/**
 *
 */
const static auto BACKGROUND_COLOR = sf::Color(64, 64, 64);

/**
 *
 */
ScreenIndex SelectTrackScreen::run(sf::RenderWindow &app) {
    sf::Event event;

    std::cout << INDEX << std::endl;

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }
        }
        app.clear(BACKGROUND_COLOR);
        app.display();
    }
}

} // end namespace linthesia

