#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "context/context.h"
#include "track_box.h"

namespace linthesia {

sf::Texture TrackBox::backgroundTexture = sf::Texture();
sf::Font TrackBox::font = sf::Font();

/**
 *
 */
void TrackBox::init() {
    //TODO: replace hardcoded strings by something better
    if (!backgroundTexture.loadFromFile("../graphics/trackbox.png")) {
        std::cerr << "Can't load trackbutton.png" << std::endl;
    }
    backgroundTexture.setSmooth(true);

    if (!font.loadFromFile("../fonts/FaunaOne-Regular.ttf")) {
        std::cerr << "Can't load font FaunaOne-Regular" << std::endl;
    }

}

/**
 *
 */
TrackBox::TrackBox(
    const std::string &instrumentName,
    unsigned notesNumber
) :
    next(IconType::NEXT, IconColor::GREY),
    previous(IconType::PREVIOUS, IconColor::GREY)
{

    const std::string instrument("Instrument:");
    const std::string notes("Notes:");
    const std::string playChoice("Play automatically");
    const unsigned BUTTON_PADDING = 20;
    const unsigned CHARACTER_SIZE = 15;
    const unsigned INTER_LINE_SPACE = 10;

    const unsigned SECOND_LINE_Y =
        BUTTON_PADDING +
        CHARACTER_SIZE +
        INTER_LINE_SPACE;

    background.setTexture(backgroundTexture);


    // init ">" button to go to next output source
    next.setPosition(
        ICON_WIDTH,
        SECOND_LINE_Y
    );

    // init "<" button to go to previous output source
    previous.setPosition(
        BUTTON_PADDING,
        SECOND_LINE_Y
    );


    instrumentLabel.setFont(font);
    instrumentLabel.setCharacterSize(CHARACTER_SIZE);
    instrumentLabel.setStyle(sf::Text::Bold);
    instrumentLabel.setString(instrument);
    instrumentLabel.setPosition(
        BUTTON_PADDING,
        BUTTON_PADDING
    );

    // init the part displaying the midi output used
    playChoiceLabel.setFont(font);
    playChoiceLabel.setCharacterSize(CHARACTER_SIZE);
    playChoiceLabel.setString(playChoice);
    playChoiceLabel.setPosition(
        BUTTON_PADDING + ICON_WIDTH,
        // we center text in the middle of the icon
        SECOND_LINE_Y + (ICON_WIDTH - CHARACTER_SIZE) / 2.0f
    );
}

/**
 *
 */
bool TrackBox::actionTriggered(
    const sf::Window &app,
    const sf::Event &event,
    linthesia::Context &context
) {
    // if "next" output button clicked
    if (next.actionTriggered(app, event, getPosition())) {
        // TODO: change context track option
        // TODO: change playChoice text
        return true;
    }

    // if "previous" button clicked
    if (previous.actionTriggered(app, event, getPosition())) {
        // TODO: change context track option
        // TODO: change playChoice text
        return true;
    }
    return false;
}



/**
 *
 */
void TrackBox::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(background, states);
    target.draw(instrumentNameLabel, states);
    target.draw(instrumentLabel, states);
    target.draw(notesLabel, states);
    target.draw(notesNumberLabel, states);
    target.draw(previous, states);
    target.draw(next, states);
    target.draw(playChoiceLabel, states);
}

} // end of namespace
