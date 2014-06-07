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
const static auto BACKGROUND_COLOR = sf::Color(64, 64, 64);

/**
 *
 */
ScreenIndex MainScreen::run(sf::RenderWindow &app) {
    sf::Event event;

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                return SelectTrackScreen::INDEX;
            }
        }
        app.clear(BACKGROUND_COLOR);
        app.display();
    }
}

} // end namespace linthesia

