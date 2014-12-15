#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "libmidi/midi_util.h"

#include "assets/path.h"
#include "context/context.h"
#include "select_midi_in.h"


namespace linthesia {
/**
 * advance the song by the given delta time
 * and play/stop the notes according to the events
 * present in that delta time
 */
static void playInput(
    linthesia::Context &context,
    const MidiEvent &event
) {
    std::cout << "We play " << event.get_note_name(event.get_note_number());
    context.midiOut.write(event);
}

/**
 *
 */
SelectMidiIn::SelectMidiIn() :
    next(IconType::NEXT, IconColor::GREY),
    previous(IconType::PREVIOUS, IconColor::GREY)
{

    const std::string title("Input Device");
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


    // init ">" button to go to next input source
    float nextYPosition =
        background.getGlobalBounds().width -
        BUTTON_PADDING -
        ICON_WIDTH;

    next.setPosition(
        nextYPosition,
        SECOND_LINE_Y
    );

    // init "<" button to go to previous input source
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

    // init the part displaying the midi input used
    inputLabel.setFont(font);
    inputLabel.setCharacterSize(CHARACTER_SIZE);
    inputLabel.setString(title);
    inputLabel.setPosition(
        BUTTON_PADDING + ICON_WIDTH,
        // we center text in the middle of the icon
        SECOND_LINE_Y + (ICON_WIDTH - CHARACTER_SIZE) / 2.0f
    );
}

/**
 *
 */
bool SelectMidiIn::actionTriggered(
    const sf::Window &app,
    const sf::Event &event,
    linthesia::Context &context
) {
    // if "next" input button clicked
    if (next.actionTriggered(app, event, getPosition())) {
        context.midiIn.switchNextPort();
        inputLabel.setString(context.midiIn.getCurrentPortName());
        // we close/open in order to use new midi port
        context.midiIn.close();
        context.midiIn.open();
        return true;
    }

    // if "previous" button clicked
    if (previous.actionTriggered(app, event, getPosition())) {
        context.midiIn.switchPrevPort();
        inputLabel.setString(context.midiIn.getCurrentPortName());
        // we close/open in order to use new midi port
        context.midiIn.close();
        context.midiIn.open();
        return true;
    }

    return false;
}

/**
 *
 */
void SelectMidiIn::run(
    const sf::Window &app,
    linthesia::Context &context,
    const sf::Time &delta
) {
    MidiEvent event = context.midiIn.read();
    while(
        event.get_type() == MidiEventType_NoteOff ||
        event.get_type() == MidiEventType_NoteOn
    ) {
        playInput(context, event);
        event = context.midiIn.read();
    }

}
/**
 *
 */
void SelectMidiIn::setInputName(const std::string &inputName) {
    inputLabel.setString(inputName);
}

/**
 *
 */
void SelectMidiIn::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(background, states);
    target.draw(titleLabel, states);
    target.draw(inputLabel, states);
    target.draw(previous, states);
    target.draw(next, states);
}

} // end of namespace

