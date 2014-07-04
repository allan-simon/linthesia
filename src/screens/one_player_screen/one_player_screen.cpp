#include <iostream>
#include <vector>

#include "one_player_screen.h"
#include "context/context.h"
#include "keyboard/white_key.h"
#include "keyboard/black_key.h"
#include "screens/select_track_screen/select_track_screen.h"

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
ScreenIndex OnePlayerScreen::run(
    sf::RenderWindow &app,
    Context &context
) {
    sf::Event event;

    sf::Clock clock;
    sf::Time currentElapsed = clock.getElapsedTime();
    sf::Time lastElapsed = clock.getElapsedTime();

    //TODO: move that into keyboard class
    // 35 because my low cost keyboard has 35 white keys (5 Octaves)...
    std::vector<WhiteKey> allWhiteKeys(35);
    const unsigned WHITE_KEY_WIDTH = 20;
    const unsigned BLACK_KEY_WIDTH = 12;
    unsigned xWhite = 0;
    for (auto &oneWhiteKey : allWhiteKeys) {
        oneWhiteKey.setPosition(xWhite, 200);
        xWhite += WHITE_KEY_WIDTH;
    };

    BlackKey blackKey;
    blackKey.setPosition(
        WHITE_KEY_WIDTH - (BLACK_KEY_WIDTH / 2.0f),
        200
    );

    bool isPlaying = false;

    context.midiOut.open();

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return STOP_APPLICATION;
            }

            if (event.type == sf::Event::KeyPressed) {
                //pressing <space> put the game in pause
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    isPlaying = !isPlaying;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    context.midiOut.close();
                    return SelectTrackScreen::INDEX;
                }
            }

        }
        app.clear(BACKGROUND_COLOR);
        // WIP replace by draw keyboard
        for (const auto& oneWhiteKey: allWhiteKeys) {
            app.draw(oneWhiteKey);
        }
        app.draw(blackKey);
        app.display();

        if (isPlaying) {
            playSong(
                context,
                currentElapsed - lastElapsed
            );
        }
        lastElapsed = currentElapsed;
        currentElapsed = clock.getElapsedTime();
    }
}

} // end namespace linthesia

