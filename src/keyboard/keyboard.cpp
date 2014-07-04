#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "keyboard.h"

namespace linthesia {

const unsigned NUMBER_OCTAVES = 5;
const unsigned NBR_WHITE_KEYS = 35;
const unsigned NBR_BLACK_KEYS = 25;
const unsigned NBR_WHITE_KEYS_BY_OCTAVE = 7;
const unsigned NBR_BLACK_KEYS_BY_OCTAVE = 5;

/**
 *
 */
Keyboard::Keyboard():
    whiteKeys(NBR_WHITE_KEYS),
    blackKeys(NBR_BLACK_KEYS)
{
    const unsigned WHITE_KEY_WIDTH = 20;
    const unsigned BLACK_KEY_WIDTH = 12;
    unsigned xWhite = 0;

    for (auto &oneWhiteKey : whiteKeys) {
        oneWhiteKey.setPosition(xWhite, 0);
        xWhite += WHITE_KEY_WIDTH;
    };


    // we draw octave by octave
    float xBlack = WHITE_KEY_WIDTH - (BLACK_KEY_WIDTH / 2.0f);
    for (
        unsigned currentOctave = 0;
        currentOctave < NUMBER_OCTAVES;
        currentOctave++
    ) {
        const unsigned offset = currentOctave * NBR_BLACK_KEYS_BY_OCTAVE;

        blackKeys[0 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        blackKeys[1 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        xBlack += WHITE_KEY_WIDTH;
        blackKeys[2 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        blackKeys[3 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        blackKeys[4 + offset].setPosition(xBlack, 0); xBlack += WHITE_KEY_WIDTH;
        xBlack += WHITE_KEY_WIDTH;
    }
}

/**
 *
 */
void Keyboard::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    for (const auto& oneWhiteKey: whiteKeys) {
        target.draw(oneWhiteKey, states);
    }
    for (const auto& oneBlackKey: blackKeys) {
        target.draw(oneBlackKey, states);
    }
}


} // end of namespace
