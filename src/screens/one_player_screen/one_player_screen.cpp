#include "libmidi/midi.h"
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

    setKeyboardPosition(app);
    setKeyboardTrailPosition(app);
    setNoteGroundPosition(app);

    noteGround.addNote();
    noteGround.render();

    bool isPlaying = false;

    context.midiOut.open();
    context.midiIn.open();

    MidiEventList inputNotes;

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
                    context.midiIn.close();
                    return SelectTrackScreen::INDEX;
                }
            }

        }
        app.clear(BACKGROUND_COLOR);
        app.draw(keyboard);
        app.draw(keyboardTrail);
        app.draw(noteGround);
        app.display();

        if (isPlaying) {
            playSong(
                context,
                currentElapsed - lastElapsed
            );
        }
        inputNotes = context.midiIn.readAllNotes();
        updateKeyboard(inputNotes, context);

        lastElapsed = currentElapsed;
        currentElapsed = clock.getElapsedTime();
    }
}

/**
 *
 */
void OnePlayerScreen::updateKeyboard(
    const MidiEventList& inputNotes,
    linthesia::Context &context
) {
    for (const auto& oneNote : inputNotes) {

        const unsigned noteNumber = oneNote.get_note_number();
        //note: on my keyboard at least, NoteOff are not sent
        //instead we got NoteOn with a velocity of 0
        const int velocity = oneNote.get_note_velocity();

        context.midiOut.write(oneNote);
        if (oneNote.is_note_on() && velocity > 0) {
            keyboard.keyPressed(noteNumber);
        }

        if (oneNote.is_note_off() || velocity == 0) {
            keyboard.keyReleased(noteNumber);
        }

    }
}

/**
 *
 */
void OnePlayerScreen::setKeyboardPosition(const sf::RenderWindow &app) {

    float yPosition = app.getSize().y -
        keyboard.getGlobalBounds().height -
        SPACE_BUTTONS;

    keyboard.setPosition(
        (app.getSize().x - keyboard.getGlobalBounds().width) / 2.0f,
        yPosition
    );
}

/**
 *
 */
void OnePlayerScreen::setKeyboardTrailPosition(
    const sf::RenderWindow &app
) {

    const float keyboardWidth = keyboard.getGlobalBounds().width;
    keyboardTrail.setWidth(static_cast<int>(keyboardWidth));

    float yPosition = app.getSize().y -
        keyboard.getGlobalBounds().height -
        keyboardTrail.getGlobalBounds().height -
        SPACE_BUTTONS;

    keyboardTrail.setPosition(
        (app.getSize().x - keyboardWidth) / 2.0f,
        yPosition
    );

}

/**
 *
 */
void OnePlayerScreen::setNoteGroundPosition(
    const sf::RenderWindow &app
) {
    const float keyboardWidth = keyboard.getGlobalBounds().width;

    float height = app.getSize().y -
        keyboard.getGlobalBounds().height -
        keyboardTrail.getGlobalBounds().height -
        noteGround.getGlobalBounds().height -
        SPACE_BUTTONS;

    noteGround.setSize(sf::Vector2f(keyboardWidth, height));

    noteGround.setPosition(
        (app.getSize().x - keyboardWidth) / 2.0f,
        0
    );




}

} // end namespace linthesia

