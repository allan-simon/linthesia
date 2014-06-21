#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "context/context.h"
#include "select_midi_out.h"

namespace linthesia {

const static unsigned ICON_WIDTH = 36;
const static unsigned ICON_HEIGHT = 36;


/**
 * code to choose the icon color
 * it follow the order in the InterfaceButtons.tga file
 */
enum IconColor {
    BLUE = 0 * ICON_WIDTH,
    GREEN = 1 * ICON_WIDTH,
    ORANGE = 2 * ICON_WIDTH,
    YELLOW = 3 * ICON_WIDTH,
    PURPLE = 4 * ICON_WIDTH,
    RED = 5 * ICON_WIDTH,
    GREY = 6 * ICON_WIDTH
};

/**
 * code to choose the icon type
 * it follow the order in the InterfaceButtons.tga file
 */
enum IconType {
    PREVIOUS = 0 * ICON_HEIGHT,
    NEXT = 1 * ICON_HEIGHT,
    SQUARRE = 2 * ICON_HEIGHT,
    SOUND = 3 * ICON_HEIGHT,
    PAUSE = 4 * ICON_HEIGHT
};

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
SelectMidiOut::SelectMidiOut() {

    const std::string title("Output Device");
    const unsigned BUTTON_PADDING = 20;
    const unsigned CHARACTER_SIZE = 15;
    const unsigned INTER_LINE_SPACE = 10;

    const unsigned SECOND_LINE_Y =
        BUTTON_PADDING +
        CHARACTER_SIZE +
        INTER_LINE_SPACE;

    //TODO: replace hardcoded strings by something better
    if (!backgroundTexture.loadFromFile("../graphics/button2line.png")) {
        std::cerr << "Can't load button2line.png" << std::endl;
    }
    backgroundTexture.setSmooth(true);
    background.setTexture(backgroundTexture);


    if (!iconsTexture.loadFromFile("../graphics/InterfaceButtons.tga")) {
        std::cerr << "Can't load InterfaceButtons.tga" << std::endl;
    }
    iconsTexture.setSmooth(true);

    // init "play" button
    float playYPosition =
        background.getGlobalBounds().width -
        BUTTON_PADDING -
        ICON_WIDTH;

    play.setTexture(iconsTexture);
    play.setTextureRect(sf::IntRect(
        IconType::SOUND,
        IconColor::GREY,
        ICON_WIDTH,
        ICON_HEIGHT
    ));
    play.setPosition(
        playYPosition,
        SECOND_LINE_Y
    );

    // init ">" button to go to next output source
    next.setTexture(iconsTexture);
    next.setTextureRect(sf::IntRect(
        IconType::NEXT,
        IconColor::GREY,
        ICON_WIDTH,
        ICON_HEIGHT
    ));
    next.setPosition(
        playYPosition - ICON_WIDTH,
        SECOND_LINE_Y
    );

    // init "<" button to go to previous output source
    previous.setTexture(iconsTexture);
    previous.setTextureRect(sf::IntRect(
        IconType::PREVIOUS,
        IconColor::GREY,
        ICON_WIDTH,
        ICON_HEIGHT
    ));
    previous.setPosition(
        BUTTON_PADDING,
        SECOND_LINE_Y
    );


    if (!font.loadFromFile("../fonts/FaunaOne-Regular.ttf")) {
        std::cerr << "Can't load font FaunaOne-Regular" << std::endl;
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
    if (buttonActionTriggered(app, event, next)) {
        context.midiOut.switchNextPort();
        outputLabel.setString(context.midiOut.getCurrentOutputName());
        // we close/open in order to use new midi port
        context.midiOut.close();
        context.midiOut.open();
        return true;
    }

    // if "previous" button clicked
    if (buttonActionTriggered(app, event, previous)) {
        context.midiOut.switchPrevPort();
        outputLabel.setString(context.midiOut.getCurrentOutputName());
        // we close/open in order to use new midi port
        context.midiOut.close();
        context.midiOut.open();
        return true;
    }

    if (buttonActionTriggered(app, event, play)) {
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
bool SelectMidiOut::buttonActionTriggered(
    const sf::Window &app,
    const sf::Event &event,
    sf::Sprite &button
) {

    if (!buttonContainsPoint(button, sf::Mouse::getPosition(app))) {
        return false;
    }
    if (
        event.type == sf::Event::MouseButtonPressed &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left)
    ) {
        return true;
    }

    return false;

}

/**
 *
 */
bool SelectMidiOut::buttonContainsPoint(
    const sf::Sprite &button,
    const sf::Vector2i &point
) const {
    // we subtract the x,y position of the button itself
    // as the point given in parameter has its coordinate relative to
    // the window
    return button.getGlobalBounds().contains(
        point.x - getPosition().x,
        point.y - getPosition().y
    );
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

