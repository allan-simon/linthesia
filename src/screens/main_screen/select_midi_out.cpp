#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "assets/path.h"
#include "context/context.h"
#include "select_midi_out.h"


namespace linthesia {
/**
 * advance the song by the given delta time
 * and play/stop the notes according to the events
 * present in that delta time
 */
static void playSong(
    linthesia::Context &context,
    const sf::Time& delta
) {
    auto events =context.update(delta.asMicroseconds());
    for (const auto& oneEvent : events) {
        context.midiOut.write(oneEvent.second);
    }
}

/**
 *
 */
SelectMidiOut::SelectMidiOut() :
    next(IconType::NEXT, IconColor::GREY),
    previous(IconType::PREVIOUS, IconColor::GREY),
    play(IconType::SOUND, IconColor::GREY)
{

    const std::string title("Output Device");
    const unsigned BUTTON_PADDING = 20;
    const unsigned CHARACTER_SIZE = 15;
    const unsigned INTER_LINE_SPACE = 10;

    const unsigned SECOND_LINE_Y =
        BUTTON_PADDING +
        CHARACTER_SIZE +
        INTER_LINE_SPACE;

    const std::string BUTTON = GRAPHICS_DIR "button2line.png";
    if (!backgroundTexture.loadFromFile(BUTTON)) {
        std::cerr
            << "Can't load "
            << BUTTON
            << std::endl
        ;
    }
    backgroundTexture.setSmooth(true);
    background.setTexture(backgroundTexture);


    // init "play" button
    float playYPosition =
        background.getGlobalBounds().width -
        BUTTON_PADDING -
        ICON_WIDTH;

    play.setPosition(
        playYPosition,
        SECOND_LINE_Y
    );

    // init ">" button to go to next output source
    next.setPosition(
        playYPosition - ICON_WIDTH,
        SECOND_LINE_Y
    );

    // init "<" button to go to previous output source
    previous.setPosition(
        BUTTON_PADDING,
        SECOND_LINE_Y
    );


    if (!font.loadFromFile(DEFAULT_FONT)) {
        std::cerr
            << "Can't load "
            << DEFAULT_FONT
            << std::endl
        ;
    }

    titleLabel.setFont(font);
    titleLabel.setCharacterSize(CHARACTER_SIZE);
    titleLabel.setStyle(sf::Text::Bold);
    titleLabel.setString(title);
    titleLabel.setPosition(
        BUTTON_PADDING,
        BUTTON_PADDING
    );

    // init the part displaying the midi output used
    outputLabel.setFont(font);
    outputLabel.setCharacterSize(CHARACTER_SIZE);
    outputLabel.setString(title);
    outputLabel.setPosition(
        BUTTON_PADDING + ICON_WIDTH,
        // we center text in the middle of the icon
        SECOND_LINE_Y + (ICON_WIDTH - CHARACTER_SIZE) / 2.0f
    );
}

/**
 *
 */
bool SelectMidiOut::actionTriggered(
    const sf::Window &app,
    const sf::Event &event,
    linthesia::Context &context
) {
    // if "next" output button clicked
    if (next.actionTriggered(app, event, getPosition())) {
        context.midiOut.switchNextPort();
        outputLabel.setString(context.midiOut.getCurrentOutputName());
        // we close/open in order to use new midi port
        context.midiOut.close();
        context.midiOut.open();
        return true;
    }

    // if "previous" button clicked
    if (previous.actionTriggered(app, event, getPosition())) {
        context.midiOut.switchPrevPort();
        outputLabel.setString(context.midiOut.getCurrentOutputName());
        // we close/open in order to use new midi port
        context.midiOut.close();
        context.midiOut.open();
        return true;
    }

    if (play.actionTriggered(app, event, getPosition())) {
        // we toggle the state pause/playing
        isPlaying = !isPlaying;
        return true;

    }
    return false;
}

/**
 *
 */
void SelectMidiOut::run(
    const sf::Window &app,
    linthesia::Context &context,
    const sf::Time &delta
) {

    if (isPlaying) {
        playSong(context, delta);
    }
}
/**
 *
 */
void SelectMidiOut::setOutputName(const std::string &outputName) {
    outputLabel.setString(outputName);
}

/**
 *
 */
void SelectMidiOut::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(background, states);
    target.draw(titleLabel, states);
    target.draw(outputLabel, states);
    target.draw(previous, states);
    target.draw(next, states);
    target.draw(play, states);
}

} // end of namespace

