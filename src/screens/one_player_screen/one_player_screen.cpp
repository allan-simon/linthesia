#include <algorithm>
#include <cmath>

#include <SFML/Graphics/View.hpp>

#include "assets/path.h"
#include "libmidi/midi.h"
#include "one_player_screen.h"
#include "context/context.h"
#include "screens/select_track_screen/select_track_screen.h"

namespace linthesia {

/**
 * we can't go over 2^4 = 16 time faster or slower
 */
const static int MAX_SPEED = 4;

/**
 *
 */
const static unsigned MICRO_SECOND_PER_PIXEL = 8000;

/**
 *
 */
const ScreenIndex OnePlayerScreen::INDEX = "one_player_screen";

/**
 *
 */
const static auto BACKGROUND_COLOR = sf::Color(64, 64, 64);

/**
 *
 */
void OnePlayerScreen::playSong(
    Context &context,
    const MidiEventListWithTrackId &events
) {
    for (const auto& oneEvent : events) {

        if (!oneEvent.second.is_playable()) {
            continue;
        }
        const unsigned trackId = oneEvent.first;

        if (!context.tracksOptions.isPlayedByComputer(trackId)) {
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
    //TODO move all these "init*" in a function made for that
    // and or see a better way to avoid that polluating this
    // function
    font.loadFromFile(DEFAULT_FONT);
    sf::Event event;

    sf::Clock clock;
    sf::Time currentElapsed = clock.getElapsedTime();
    sf::Time lastElapsed = clock.getElapsedTime();

    score.reset();
    scoreDisplay.init();
    scoreDisplay.update(score);

    initSpeedLabel(app);
    initInfoLabel(app);

    // we make sure the song is reset at the beginning
    // (fix #54)
    context.resetSong();
    auto microSecBeforeStart = context.getMicroSecondBeforeStart();

    nonScrolledMicroSec = 0;


    setScoreDisplayPosition(app);
    setKeyboardPosition(app);
    setKeyboardTrailPosition(app);

    initNoteGround(context);
    setNoteGroundView(app);

    notesTracker.fill(context);

    noteGround.moveAndReRenderIfNecessary(0, context);

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

                // pressing <left> increase song speed
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    increaseSpeed();
                }

                // pressing <right> decrease song speed
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    decreaseSpeed();
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

        app.draw(scoreDisplay);
        app.draw(speedLabel);
        app.draw(info);

        // the note ground is displayed in the view
        // so that we can scroll it
        app.setView(noteGroundView);
        app.draw(noteGround);
        app.setView(app.getDefaultView());

        app.display();

        auto delta = currentElapsed - lastElapsed;
        lastElapsed = currentElapsed;
        currentElapsed = clock.getElapsedTime();

        inputNotes = context.midiIn.readAllNotes();
        updateKeyboard(inputNotes, context);
        playInputNotes(context, inputNotes);

        if (!isPlaying) {
            continue;
        }

        delta *= std::pow(2.0f, static_cast<float>(speedFactor));

        scrollNoteGround(
            MICRO_SECOND_PER_PIXEL,
            delta,
            app,
            context
        );

        // we start the song only when the "time to get ready"
        // is over
        if (microSecBeforeStart > delta.asMicroseconds()) {
            microSecBeforeStart -= delta.asMicroseconds();
            continue;
        }

        auto events = context.update(delta.asMicroseconds());


        playSong(context, events);

        auto missedNotes = notesTracker.checkMissedNotes(
            context.getSongPosition()
        );
        score.missed += missedNotes.size();
        checkHitNotes(inputNotes, context);
        scoreDisplay.update(score);

    }
}

/**
 *
 */
void OnePlayerScreen::initNoteGround(
    const Context &context
) {
    noteGround.setMicrosecondBeforeStart(
        context.getMicroSecondBeforeStart()
    );
    noteGround.init(
        Keyboard::NBR_WHITE_KEYS,
        MICRO_SECOND_PER_PIXEL
    );

    noteGround.addNotes(context.getNotes());
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
void OnePlayerScreen::playInputNotes(
    linthesia::Context &context,
    const MidiEventList& inputNotes
) {
    for (const auto& oneNote : inputNotes) {
        context.midiOut.write(oneNote);
    }
}

/**
 *
 */
void OnePlayerScreen::checkHitNotes(
    const MidiEventList& inputNotes,
    linthesia::Context &context
) {
    for (const auto& oneNote : inputNotes) {

        const unsigned noteNumber = oneNote.get_note_number();
        //note: on my keyboard at least, NoteOff are not sent
        //instead we got NoteOn with a velocity of 0
        const int velocity = oneNote.get_note_velocity();

        if (oneNote.is_note_on() && velocity > 0) {
            auto delta = notesTracker.hitNote(
                noteNumber,
                context.getSongPosition()
            );
            score.markNote(delta);
        }

        if (oneNote.is_note_off() || velocity == 0) {
            //TODO: handle released note
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
void OnePlayerScreen::setScoreDisplayPosition(const sf::RenderWindow &app) {

    float yPosition = app.getSize().y -
        SPACE_BUTTONS;

    scoreDisplay.setPosition(
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
void OnePlayerScreen::setNoteGroundView(
    const sf::RenderWindow &app
) {
    const float keyboardWidth = keyboard.getGlobalBounds().width;

    //height of the noteGroundView is the space remaining in the application
    // after we remove...
    const float height = app.getSize().y -
        // height of keyboard trail ...
        keyboardTrail.getGlobalBounds().height -
        // height of keyboard itself ...
        keyboard.getGlobalBounds().height -
        // and the spaces left for buttons at the bottom
        SPACE_BUTTONS;


    noteGroundView.reset(sf::FloatRect(
        // we put the view at the bottom of note ground with the equivalent
        // (minus the dimension of the view itself)
        // so that we show the first notes
        noteGround.getGlobalBounds().width - keyboardWidth,
        noteGround.getGlobalBounds().height - height,
        //
        keyboardWidth,
        height
    ));

    const float noteGroundXRatio = keyboardWidth / app.getSize().x;
    const float noteGroundYRatio = height / app.getSize().y;
    noteGroundView.setViewport(sf::FloatRect(
        // center in middle of window on X axis
        (1 - noteGroundXRatio) / 2.0f,
        //put at top of the window
        0,
        noteGroundXRatio,
        noteGroundYRatio
    ));
}

/**
 *
 */
void OnePlayerScreen::scrollNoteGround(
    const unsigned microSecondPerPixel,
    const sf::Time& delta,
    const sf::RenderWindow &app,
    const Context& context
) {
    auto deltaMicroSec = delta.asMicroseconds();

    // fix #55, as for each 'refresh' of the screen we move the note
    // ground by an integer number of pixel, we need to keep track
    // of the micro second non 'consumed' for the next refresh
    // otherwise the ground will move "slower" than the actual song
    deltaMicroSec += nonScrolledMicroSec;
    nonScrolledMicroSec = deltaMicroSec % microSecondPerPixel;

    auto offsetY = deltaMicroSec / microSecondPerPixel;

    // it's -offsetY because we're scrolling up
    bool needToSwitchScreen = noteGround.moveAndReRenderIfNecessary(
        offsetY,
        context
    );
    noteGroundView.move(0, -offsetY);
    if (needToSwitchScreen) {
        setNoteGroundView(app);
    }

}

/**
 *
 */
void OnePlayerScreen::initSpeedLabel(const sf::RenderWindow &app) {

    if (!font.loadFromFile(DEFAULT_FONT)) {
        std::cerr
            << "Can't load "
            << DEFAULT_FONT
            << std::endl
        ;
    }
    speedLabel.setFont(font);
    speedLabel.setColor(sf::Color(200, 165, 0));
    updateSpeedLabel();

    float yPosition = app.getSize().y - SPACE_BUTTONS / 2;

    speedLabel.setPosition(
        (app.getSize().x - keyboard.getGlobalBounds().width) / 2.0f,
        yPosition
    );
}

/**
 *
 */
void OnePlayerScreen::initInfoLabel(const sf::RenderWindow &app) {

    if (!font.loadFromFile(DEFAULT_FONT)) {
        std::cerr
            << "Can't load "
            << DEFAULT_FONT
            << std::endl
        ;
    }
    info.setFont(font);
    info.setColor(sf::Color(200, 165, 0));
    info.setString("press space to start");

    sf::Vector2<unsigned int> windowSize = app.getSize();

    auto keyboardWidth = keyboard.getGlobalBounds().width;

    info.setPosition(
        (windowSize.x - keyboardWidth) / 2.0f + INFO_LABEL_X_OFFSET,
        (windowSize.y - SPACE_BUTTONS / 2)
    );
}

/**
 *
 */
void OnePlayerScreen::increaseSpeed() {
    speedFactor =  std::min(MAX_SPEED, speedFactor+1);
    updateSpeedLabel();
}

/**
 *
 */
void OnePlayerScreen::decreaseSpeed() {
    speedFactor =  std::max(-MAX_SPEED, speedFactor-1);
    updateSpeedLabel();
}

/**
 *
 */
void OnePlayerScreen::updateSpeedLabel() {

    //TODO quick and dirty solution
    std::string text;
    if (speedFactor == -4) { text = " 1/16"; }
    if (speedFactor == -3) { text = " 1/8"; }
    if (speedFactor == -2) { text = " 1/4"; }
    if (speedFactor == -1) { text = " 1/2"; }
    if (speedFactor == 0) { text = " 1"; }
    if (speedFactor == 1) { text = " 2"; }
    if (speedFactor == 2) { text = " 4"; }
    if (speedFactor == 3) { text = " 8"; }
    if (speedFactor == 4) { text = "16"; }

    speedLabel.setString("speed:" + text);
}

} // end namespace linthesia

