#include <iostream>
#include <vector>

#include "one_player_screen.h"
#include "context/context.h"
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
void OnePlayerScreen::playSong(
    linthesia::Context &context,
    const sf::Time& delta
) {
    auto events = context.update(delta.asMicroseconds());
    for (const auto& oneEvent : events) {

        if (!oneEvent.second.is_playable()) {
            continue;
        }
        const unsigned char noteChannel = oneEvent.second.get_channel();
        const unsigned noteNumber = oneEvent.second.get_note_number();
        const sf::Color color = context.getChannelColor(noteChannel);

        if (!context.tracksOptions.isPlayedByComputer(noteChannel)) {
            continue;
        }

        context.midiOut.write(oneEvent.second);

        if (oneEvent.second.is_note_on()) {
            keyboard.keyPressed(noteNumber, color);
        }

        if (oneEvent.second.is_note_off()) {
            keyboard.keyReleased(noteNumber);
        }
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
        app.draw(keyboard);
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

