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
    exitButton.setPosition(
        0,
        app.getSize().y - exitButton.getGlobalBounds().height
    );
    //TODO: add real position to this button

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                //TODO: maybe create directly a "isClicked" function ?
                if (exitButton.containsPoint(sf::Mouse::getPosition(app))) {
                    return STOP_APPLICATION;
                }
                return SelectTrackScreen::INDEX;
            }
        }
        app.clear(BACKGROUND_COLOR);
        app.draw(exitButton);
        app.display();
    }
}

} // end namespace linthesia

