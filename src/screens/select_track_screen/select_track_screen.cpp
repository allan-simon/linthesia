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
 * advance the song by the given delta time
 * and play/stop the notes according to the events
 * present in that delta time
 */
static void playSong(
    linthesia::Context &context,
    const sf::Time& delta
) {
    auto events = context.update(delta.asMicroseconds());
    for (const auto& oneEvent : events) {
        unsigned char noteChannel = oneEvent.second.get_channel();
        if (!context.tracksOptions.isPlayedByComputer(noteChannel)) {
            continue;
        }
        context.midiOut.write(oneEvent.second);
    }
}



/**
 *
 */
SelectTrackScreen::SelectTrackScreen() :
    AbstractScreen(),
    backButton("back"),
    startGameButton("start game")
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

    sf::Clock clock;
    sf::Time currentElapsed = clock.getElapsedTime();
    sf::Time lastElapsed = clock.getElapsedTime();



    setBackButtonPosition(app);
    setStartGameButtonPosition(app);

    allTrackBoxes.clear();
    auto tracks = context.getTracks();
    for (const auto& oneTrack : tracks) {

        // we don't display any option for track without notes
        if (oneTrack.aggregate_notes_count() == 0) {
            continue;
        }

        allTrackBoxes.emplace_back(
            oneTrack.get_instrument_name(),
            oneTrack.aggregate_notes_count(),
            oneTrack.get_track_id()
        );
    }
    setTrackBoxesPosition(app);

    context.midiOut.open();

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }

            if (backButton.actionTriggered(app, event)) {
                return START_APPLICATION;
            }

            for (auto& oneTrackBox : allTrackBoxes) {
                oneTrackBox.actionTriggered(
                    app,
                    event,
                    context.tracksOptions
                );
            }

            if (startGameButton.actionTriggered(app, event)) {
                isPlaying = true;
            }
        }


        if (isPlaying) {
            playSong(
                context,
                currentElapsed - lastElapsed
            );
        }
        lastElapsed = currentElapsed;
        currentElapsed = clock.getElapsedTime();


        app.clear(BACKGROUND_COLOR);
        app.draw(backButton);
        app.draw(startGameButton);
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
void SelectTrackScreen::setStartGameButtonPosition(const sf::RenderWindow &app) {
    auto appSize = app.getSize();
    auto buttonBound = startGameButton.getGlobalBounds();

    startGameButton.setPosition(
        appSize.x - (buttonBound.width + PADDING) ,
        appSize.y - (buttonBound.height + PADDING)
    );

}

/**
 *
 */
void SelectTrackScreen::setTrackBoxesPosition(const sf::RenderWindow &app) {

    if (allTrackBoxes.size() == 0) {
        return;
    }

    unsigned TRACK_BOX_MARGIN = 20;
    float TRACK_BOX_HEIGHT = allTrackBoxes[0].getGlobalBounds().height;
    float TRACK_BOX_WIDTH = allTrackBoxes[0].getGlobalBounds().width;

    float firstX = (app.getSize().x / 2.0f) - TRACK_BOX_MARGIN - TRACK_BOX_WIDTH;
    float secondX = (app.getSize().x / 2.0f) + TRACK_BOX_MARGIN;

    unsigned currentIndex = 0;

    float usedY = 0;

    for (auto& trackBox: allTrackBoxes) {

        float usedX = secondX;
        if ((currentIndex % 2) == 0) {
            usedX = firstX;
            usedY += TRACK_BOX_HEIGHT + TRACK_BOX_MARGIN;
        }

        trackBox.setPosition(
            usedX,
            usedY
        );

        currentIndex++;
    }
}


} // end namespace linthesia

