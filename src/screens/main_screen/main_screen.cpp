#include "main_screen.h"
#include "screens/select_track_screen/select_track_screen.h"

#include "buttons/abstract_button.h"

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


    //TODO: i18n it
    linthesia::AbstractButton exitButton("exit game");
    // we set the exit button to bottom left (with padding)
    exitButton.setPosition(
        PADDING,
        app.getSize().y - (exitButton.getGlobalBounds().height + PADDING)
    );

    // on purpose infinite loop
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }

            if (exitButton.actionTriggered(app)) {
                return STOP_APPLICATION;
            }

        }
        app.clear(BACKGROUND_COLOR);
        app.draw(exitButton);
        app.display();
    }
}

} // end namespace linthesia

