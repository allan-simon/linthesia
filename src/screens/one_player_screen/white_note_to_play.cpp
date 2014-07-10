#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "white_note_to_play.h"
#include "note_ground.h"
#include "keyboard/white_key.h"

namespace linthesia {
/**
 *
 */
WhiteNoteToPlay::WhiteNoteToPlay(
    const unsigned height,
    const sf::Color &color
) {

    //the border is actually a rectangle larger
    //than the "body" itself, but as we display the body
    //over the border rectangle, it gives the impression of
    //having just a line
    //the advantages of doing so, rather than drawing a "real" border
    //of some pixels, is that we can in one time, by playing with width etc.
    //create a 2 side border effect with still 1 border rectangle

    border.setSize(sf::Vector2f(
        WhiteKey::WHITE_KEY_WIDTH - NoteGround::NOTE_SEPARATOR_WIDTH,
        height
    ));

    // half-transparent blank
    border.setFillColor(sf::Color(
        0,
        0,
        0,
        160
    ));

    body.setSize(sf::Vector2f(
        WhiteKey::WHITE_KEY_WIDTH - NoteGround::NOTE_SEPARATOR_WIDTH,
        height - 2
    ));
    body.setPosition(
        0,
        NoteGround::NOTE_SEPARATOR_WIDTH
    );
    body.setFillColor(color);
}

/**
 *
 */
void WhiteNoteToPlay::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(border, states);
    target.draw(body, states);
}



} // end namespace

