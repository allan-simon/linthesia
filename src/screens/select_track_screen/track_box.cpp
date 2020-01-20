#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "assets/path.h"
#include "track_options/track_options.h"
#include "track_box.h"

namespace linthesia {

sf::Texture TrackBox::backgroundTexture = sf::Texture();
sf::Font TrackBox::font = sf::Font();

/**
 *
 */
void TrackBox::init() {
    const std::string TRACKBOX_FILE = GRAPHICS_DIR "trackbox.png";
    if (!backgroundTexture.loadFromFile(TRACKBOX_FILE)) {
        std::cerr
            << "Can't load "
            << TRACKBOX_FILE
            << std::endl
        ;
    }
    backgroundTexture.setSmooth(true);

    if (!font.loadFromFile(DEFAULT_FONT)) {
        std::cerr
            << "Can't load "
            << DEFAULT_FONT
            << std::endl
        ;
    }

}

/**
 *
 */
sf::FloatRect TrackBox::getGlobalBounds() const {
    return background.getGlobalBounds();
}

/**
 *
 */
TrackBox::TrackBox(
    const std::string &instrumentName,
    unsigned notesNumber,
    unsigned _trackId
) :
    trackId(_trackId),
    next(IconType::NEXT, IconColor::GREY),
    previous(IconType::PREVIOUS, IconColor::GREY)
{

    const std::string instrument("Instrument:");
    const std::string notes("Notes:");
    const unsigned BUTTON_PADDING = 15;
    const unsigned CHARACTER_SIZE = 15;
    const unsigned INTER_LINE_SPACE = 10;

    background.setTexture(backgroundTexture);

    const float CHOICE_LINE_Y =
        background.getGlobalBounds().height -
        BUTTON_PADDING -
        ICON_HEIGHT;


    // init ">" button to go to next output source
    next.setPosition(
        // top right with a little padding
        background.getGlobalBounds().width - BUTTON_PADDING - ICON_WIDTH,
        CHOICE_LINE_Y
    );

    // init "<" button to go to previous output source
    previous.setPosition(
        BUTTON_PADDING,
        CHOICE_LINE_Y
    );


    instrumentLabel.setFont(font);
    instrumentLabel.setCharacterSize(CHARACTER_SIZE);
    instrumentLabel.setStyle(sf::Text::Bold);
    instrumentLabel.setString(instrument);
    instrumentLabel.setPosition(
        BUTTON_PADDING,
        BUTTON_PADDING
    );

    // X position of the title values (number of notes, name of the instrument
    const float TITLE_VALUES_X =
        BUTTON_PADDING +
        instrumentLabel.getGlobalBounds().width +
        INTER_LINE_SPACE;


    instrumentNameLabel.setFont(font);
    instrumentNameLabel.setCharacterSize(CHARACTER_SIZE);
    instrumentNameLabel.setString(instrumentName);
    instrumentNameLabel.setPosition(
        TITLE_VALUES_X,
        BUTTON_PADDING
    );




    const float NOTES_LABEL_Y =
        BUTTON_PADDING +
        CHARACTER_SIZE +
        INTER_LINE_SPACE;


    notesLabel.setFont(font);
    notesLabel.setCharacterSize(CHARACTER_SIZE);
    notesLabel.setStyle(sf::Text::Bold);
    notesLabel.setString(notes);
    notesLabel.setPosition(
        BUTTON_PADDING,
        NOTES_LABEL_Y
    );

    notesNumberLabel.setFont(font);
    notesNumberLabel.setCharacterSize(CHARACTER_SIZE);
    notesNumberLabel.setString(std::to_string(notesNumber));
    notesNumberLabel.setPosition(
        TITLE_VALUES_X,
        NOTES_LABEL_Y
    );



    playChoiceLabel.setFont(font);
    playChoiceLabel.setCharacterSize(CHARACTER_SIZE);
    // "playChoiceLabel.setString()" will be called when the track will have been chosen
    playChoiceLabel.setPosition(
        BUTTON_PADDING + ICON_WIDTH,
        // we center text in the middle of the icon
        CHOICE_LINE_Y + (ICON_WIDTH - CHARACTER_SIZE) / 2.0f
    );
}

void TrackBox::updatePlayChoiceLabel(
    linthesia::TrackOptions  &trackOptions
) {
        const std::string style = trackOptions.getStrStyle(trackId);
        playChoiceLabel.setString(style);
}

/**
 *
 */
bool TrackBox::actionTriggered(
    const sf::Window &app,
    const sf::Event &event,
    linthesia::TrackOptions  &trackOptions
) {
    // if "next" output button clicked
    if (next.actionTriggered(app, event, getPosition())) {
        const std::string style = trackOptions.toNextStyle(trackId);
        playChoiceLabel.setString(style);
        return true;
    }

    // if "previous" button clicked
    if (previous.actionTriggered(app, event, getPosition())) {
        const std::string style = trackOptions.toPreviousStyle(trackId);
        playChoiceLabel.setString(style);
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
