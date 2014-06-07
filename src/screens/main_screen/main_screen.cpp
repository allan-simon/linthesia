#include "main_screen.h"
#include "screens/select_track_screen/select_track_screen.h"

namespace linthesia {

/**
 *
 */
const ScreenIndex MainScreen::INDEX = START_APPLICATION;

/**
 *
 */
ScreenIndex MainScreen::run(sf::Window &app) {
    sf::Event event;

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }

            if (event.type == sf::Event::MouseLeft) {
                return SelectTrackScreen::INDEX;
            }
        }
    }
}

} // end namespace linthesia

