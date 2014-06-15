#include <iostream>

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
MainScreen::MainScreen() :
    AbstractScreen(),
    exitButton("exit game")
{
    //TODO: replace hardcoded strings by something better
    if (!logoTexture.loadFromFile("../graphics/title_Logo.png")) {
        std::cerr << "Can't load title_Logo.png" << std::endl;
    }
    logoTexture.setSmooth(true);
    logo.setTexture(logoTexture);

}

/**
 *
 */
ScreenIndex MainScreen::run(sf::RenderWindow &app) {
    sf::Event event;

    setExitButtonPosition(app);
    setLogoPosition(app);
    //TODO: i18n it
    // we set the exit button to bottom left (with padding)
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
        app.draw(logo);
        app.display();
    }
}

/**
 *
 */
void MainScreen::setExitButtonPosition(sf::RenderWindow &app) {
    exitButton.setPosition(
        PADDING,
        app.getSize().y - (exitButton.getGlobalBounds().height + PADDING)
    );
}

/**
 *
 */
void MainScreen::setLogoPosition(sf::RenderWindow &app) {
    logo.setPosition(
        (app.getSize().x - logo.getGlobalBounds().width) / 2.0f,
        PADDING
    );
}


} // end namespace linthesia

