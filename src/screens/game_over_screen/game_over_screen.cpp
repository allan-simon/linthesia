#include <iostream>

#include "game_over_screen.h"

namespace linthesia {

/**
 *
 */
const ScreenIndex GameOverScreen::INDEX = "game_over_screen";

/**
 *
 */
const static auto BACKGROUND_COLOR = sf::Color(64, 64, 64);

/**
 *
 */
ScreenIndex GameOverScreen::run(
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

