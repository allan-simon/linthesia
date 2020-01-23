#include <iostream>

#include "main_screen.h"
#include "screens/select_track_screen/select_track_screen.h"
#include "screens/file_select_screen/file_select_screen.h"
#include "screens/select_track_screen/select_track_screen.h"

#include "utilities/util.h"
#include "assets/path.h"
#include "context/context.h"

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
 * Space to put between buttons
 */
const unsigned INTER_BUTTON_SPACING = 15;

/**
 *
 */
MainScreen::MainScreen() :
    AbstractScreen(),
    exitButton("exit game"),
    selectTrackButton("play"),
    chooseSongButton("song:", "no song selected yet")
{
    std::string actual_graphics_dir_path;
    if (dirExists(GRAPHICS_DIR))
    {
        actual_graphics_dir_path = GRAPHICS_DIR;
    }
    else
    {
        actual_graphics_dir_path = LOCAL_GRAPHICS_DIR;
    }
    const std::string LOGO = actual_graphics_dir_path + "title_Logo.png";
    if (!logoTexture.loadFromFile(LOGO)) {
        std::cerr
            << "Can't load "
            << LOGO
            << std::endl
        ;
    }
    logoTexture.setSmooth(true);
    logo.setTexture(logoTexture);

}

/**
 *
 */
ScreenIndex MainScreen::run(
    sf::RenderWindow &app,
    Context &context
) {
    sf::Event event;
    sf::Clock clock;
    sf::Time currentElapsed = clock.getElapsedTime();
    sf::Time lastElapsed = clock.getElapsedTime();

    // note: because of a bug in version 2.0.1 of rtmidi
    // we HAVE to open Out AFTER In , otherwise In will get
    // not input MIDI messages
    context.midiIn.open();
    context.midiOut.open();

    //TODO replace by function more specific "isSongChosen""
    // something like that
    // if a song is selected we display its name on
    // choose song button
    if(!context.getFilename().empty()) {
        currentElapsed = clock.getElapsedTime();
        lastElapsed = currentElapsed;
        chooseSongButton.setText(context.getFilename());
        selectTrackButton.setActive();
    } else {
        selectTrackButton.setInactive();
    }

    setExitButtonPosition(app);
    setLogoPosition(app);
    setChooseSongButtonPosition(app);
    setSelectTrackButtonPosition(app);

    selectMidiOut.setOutputName(context.midiOut.getCurrentOutputName());
    setSelectMidiOutPosition(app);

    selectMidiIn.setInputName(context.midiIn.getCurrentPortName());
    setSelectMidiInPosition(app);


    // on purpose infinite loop
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                context.midiOut.close();
                context.midiIn.close();
                return STOP_APPLICATION;
            }

            if (exitButton.actionTriggered(app, event)) {
                context.midiOut.close();
                context.midiIn.close();
                return STOP_APPLICATION;
            }

            if (chooseSongButton.actionTriggered(app, event)) {
                context.midiOut.close();
                context.midiIn.close();
                return FileSelectScreen::INDEX;
            }

            if (selectTrackButton.actionTriggered(app, event)) {
                context.midiOut.close();
                context.midiIn.close();
                return SelectTrackScreen::INDEX;
            }

            selectMidiOut.actionTriggered(app, event, context);
            selectMidiIn.actionTriggered(app, event, context);
        }

        selectMidiOut.run(
            app,
            context,
            currentElapsed - lastElapsed
        );
        selectMidiIn.run(
            app,
            context,
            currentElapsed - lastElapsed
        );


        lastElapsed = currentElapsed;
        currentElapsed = clock.getElapsedTime();

        app.clear(BACKGROUND_COLOR);
        app.draw(exitButton);
        app.draw(selectTrackButton);
        app.draw(selectMidiOut);
        app.draw(selectMidiIn);
        app.draw(chooseSongButton);
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

/**
 *
 */
void MainScreen::setSelectTrackButtonPosition(sf::RenderWindow &app) {
    auto appSize = app.getSize();
    auto buttonBound = selectTrackButton.getGlobalBounds();

    selectTrackButton.setPosition(
        appSize.x - (buttonBound.width + PADDING) ,
        appSize.y - (buttonBound.height + PADDING)
    );
}

/**
 *
 */
void MainScreen::setChooseSongButtonPosition(sf::RenderWindow &app) {
    // we display the "choose song" button horizontally centered
    // and just below the logo
    chooseSongButton.setPosition(
        (app.getSize().x - chooseSongButton.getGlobalBounds().width) / 2.0f,
        logo.getPosition().y + logo.getGlobalBounds().height
    );
}

/**
 *
 */
void MainScreen::setSelectMidiOutPosition(const sf::RenderWindow &app) {

    float yPosition =
        chooseSongButton.getPosition().y +
        chooseSongButton.getGlobalBounds().height +
        INTER_BUTTON_SPACING;

    selectMidiOut.setPosition(
        (app.getSize().x - chooseSongButton.getGlobalBounds().width) / 2.0f,
        yPosition
    );
}

/**
 *
 */
void MainScreen::setSelectMidiInPosition(const sf::RenderWindow &app) {

    float yPosition =
        selectMidiOut.getPosition().y +
        chooseSongButton.getGlobalBounds().height +
        INTER_BUTTON_SPACING;

    selectMidiIn.setPosition(
        (app.getSize().x - chooseSongButton.getGlobalBounds().width) / 2.0f,
        yPosition
    );
}

} // end namespace linthesia

