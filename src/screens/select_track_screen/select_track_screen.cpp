#include <iostream>

#include "select_track_screen.h"
#include "context/context.h"
#include "track_box.h"

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

    auto tracks = context.getTracks();
    for (const auto& oneTrack : tracks) {
        allTrackBoxes.emplace_back(
            oneTrack.get_instrument_name(),
            oneTrack.aggregate_notes_count()
        );
    }
    setTrackBoxesPosition(app);

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
        for (const auto &trackBox : allTrackBoxes) {
            app.draw(trackBox);
        }
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

/**
 *
 */
void SelectTrackScreen::setTrackBoxesPosition(const sf::RenderWindow &app) {
    for (auto& trackBox: allTrackBoxes) {

        float yPosition = 300;

        trackBox.setPosition(
            (app.getSize().x - 100) / 2.0f,
            yPosition
        );
    }
}


} // end namespace linthesia

