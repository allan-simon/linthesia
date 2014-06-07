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
    }
}

} // end namespace linthesia

