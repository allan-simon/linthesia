#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "note_ground.h"
#include "white_note_to_play.h"
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
    unsigned index = Keyboard::noteToKeyboardIndex(noteNumber);

    if (Keyboard::isBlackKey(noteNumber)) {
        return;
    }
    WhiteNoteToPlay testNote(
        (end - start) / MICRO_SECOND_PER_PIXEL,
        sf::Color(230, 20, 20)
    );
    testNote.setPosition(
       WhiteKey::WHITE_KEY_WIDTH * index,
       start / MICRO_SECOND_PER_PIXEL
    );
    ground.draw(testNote);
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
