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
SelectTrackScreen::SelectTrackScreen() :
    AbstractScreen(),
    backButton("back")
{

}

/**
 *
 */
ScreenIndex SelectTrackScreen::run(
    sf::RenderWindow &app,
    Context &context
) {
    sf::Event event;

    setBackButtonPosition(app);

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }

            if (backButton.actionTriggered(app, event)) {
                return START_APPLICATION;
            }


        }

        app.clear(BACKGROUND_COLOR);
        app.draw(backButton);
        app.display();

    }

}

/**
 *
 */
void SelectTrackScreen::setBackButtonPosition(const sf::RenderWindow &app) {
    // put the "back" putton at bottom left (with some padding)
    backButton.setPosition(
        PADDING,
        app.getSize().y - (backButton.getGlobalBounds().height + PADDING)
    );
}



} // end namespace linthesia

