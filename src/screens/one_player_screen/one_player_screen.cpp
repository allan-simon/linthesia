#include <iostream>

#include "one_player_screen.h"

namespace linthesia {

/**
 *
 */
const ScreenIndex OnePlayerScreen::INDEX = "one_player_screen";

/**
 *
 */
const static auto BACKGROUND_COLOR = sf::Color(64, 64, 64);

/**
 *
 */
ScreenIndex OnePlayerScreen::run(
    sf::RenderWindow &app,
    Context &context
) {
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

