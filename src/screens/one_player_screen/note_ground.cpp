#include <algorithm> // for std::min
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "note_ground.h"
#include "white_note_to_play.h"
#include "black_note_to_play.h"
#include "keyboard/keyboard.h"
#include "keyboard/white_key.h"

namespace linthesia {

const static unsigned MICRO_SECOND_PER_PIXEL = 8000;

/**
 *
 */
NoteGround::NoteGround() {
    groundBackground.setFillColor(sf::Color(95, 95, 95));
    noteSeparator.setFillColor(sf::Color(62, 62, 62));
}

/**
 *
 */
sf::FloatRect NoteGround::getGlobalBounds() const {
    return groundBackground.getGlobalBounds();
}

/**
 *
 */
void NoteGround::setSize(
    const sf::Vector2f &size
) {

    const unsigned roundedWidth = static_cast<unsigned>(size.x);
    const unsigned roundedHeight = static_cast<unsigned>(size.y);
    groundBackground.setSize(size);
    ground.create(roundedWidth, roundedHeight);

    noteSeparator.setSize(sf::Vector2f(1, size.y));

    //TODO: we certainly can make it in a less hackish way
    //by giving the number of key and size of one key directly in parameter
    const unsigned sizeOneKey = roundedWidth / Keyboard::NBR_WHITE_KEYS;

    ground.draw(groundBackground);
    for (
        unsigned offset = sizeOneKey;
        offset < roundedWidth;
        offset += sizeOneKey
    ) {
        noteSeparator.setPosition(
            offset - NoteGround::NOTE_SEPARATOR_WIDTH,
            0
        );
        //note: we need to draw in that intermediate renderer
        //because we can't change the position of noteSepator
        //in draw as it's a "const" function
        //(or we need a vector of noteSeparator which I find inelegant
        ground.draw(noteSeparator);

    }

}

/**
 *
 */
void NoteGround::render() {
    sprite.setTexture(ground.getTexture());
}

/**
 *
 */
void NoteGround::addNote(
    const unsigned noteNumber,
    const unsigned start,
    const unsigned end,
    const sf::Color &color
) {
    if (Keyboard::isOutOfKeyboard(noteNumber)) {
        return;
    }
    const unsigned index = Keyboard::noteToKeyboardIndex(noteNumber);

    const unsigned noteHeight = (end - start) / MICRO_SECOND_PER_PIXEL;
    const unsigned noteYPosition = start / MICRO_SECOND_PER_PIXEL;

    if (Keyboard::isBlackKey(noteNumber)) {
        //TODO: 140 because in getChannelColor , primary is set to 200
        //and in RGB,  140 will be darker than 200
        sf::Uint8 darker = 140;
        BlackNoteToPlay testNote(
            noteHeight,
            sf::Color(
                // TODO: original color but darker
                // find better method to do that
                std::min(darker, color.r),
                std::min(darker, color.g),
                std::min(darker, color.b)
            )
        );
        testNote.setPosition(
            Keyboard::xPositionBlackNote(noteNumber),
            noteYPosition
        );
        ground.draw(testNote);
    } else {
        WhiteNoteToPlay testNote(
            noteHeight,
            color
        );
        testNote.setPosition(
           WhiteKey::WHITE_KEY_WIDTH * index,
           noteYPosition
        );
        ground.draw(testNote);
    }
}

/**
 *
 */
void NoteGround::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {

    states.transform *= getTransform();
    target.draw(sprite, states);
}

}
