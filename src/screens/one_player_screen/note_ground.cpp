#include <algorithm> // for std::min
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "note_ground.h"
#include "white_note_to_play.h"
#include "black_note_to_play.h"
#include "keyboard/keyboard.h"
#include "keyboard/white_key.h"

namespace linthesia {

//Note: in other places we divide the HEIGHT by two in order
//to have the second half of Nth screen and first half of N+1th screen
//overlapping so that we have a invisible transition between the two
const unsigned HEIGHT = 2048;

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
void NoteGround::setMicrosecondBeforeStart(
    const unsigned _microsecondBeforeStart
) {
    microSecondBeforeStart = _microsecondBeforeStart;
}

/**
 *
 */
void NoteGround::init(
    const unsigned _keyboardNbrKeys,
    const unsigned _microSecondPerPixel
) {

    keyboardNbrKeys = _keyboardNbrKeys;
    microSecondPerPixel = _microSecondPerPixel;
    elaspedPixel = 0;
    notesByHalfScreens.clear();

    const unsigned width = keyboardNbrKeys * WhiteKey::WHITE_KEY_WIDTH;
    groundBackground.setSize(sf::Vector2f(
        width,
        HEIGHT
    ));
    ground.create(width, HEIGHT);
    noteSeparator.setSize(sf::Vector2f(1, HEIGHT));
    drawBackground();
}

/**
 *
 */
void NoteGround::drawBackground() {
    const unsigned width = keyboardNbrKeys * WhiteKey::WHITE_KEY_WIDTH;
    ground.draw(groundBackground);
    for (
        unsigned offset = WhiteKey::WHITE_KEY_WIDTH;
        offset < width;
        offset += WhiteKey::WHITE_KEY_WIDTH
    ) {
        noteSeparator.setPosition(
            offset - NoteGround::NOTE_SEPARATOR_WIDTH,
            0
        );
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
bool NoteGround::moveAndReRenderIfNecessary(
    const unsigned offset,
    const Context& context
) {
    //TODO: it's not that good as now NoteGround becomes statefull
    // as we keep track internally of how many pixel has been scrolled
    unsigned previousScreen = elaspedPixel / (HEIGHT/2);
    elaspedPixel += offset;
    unsigned currentScreen = elaspedPixel / (HEIGHT/2);

    if (
        elaspedPixel != offset &&
        previousScreen == currentScreen
    ) {
        return false;
    }

    std::cout << "currentScreen " << currentScreen << std::endl;
    drawBackground();
    // fix #76: we make sure we will not go one screen too far
    if (currentScreen + 1 >= notesByHalfScreens.size()) {
        return false;
    }

    for(const auto& oneNote : notesByHalfScreens[currentScreen]) {
        addNote(
            oneNote.noteId,
            oneNote.start,
            oneNote.end,
            context.getChannelColor(oneNote.channel),
            currentScreen
        );
    }

    for(const auto& oneNote : notesByHalfScreens[currentScreen + 1]) {
        addNote(
            oneNote.noteId,
            oneNote.start,
            oneNote.end,
            context.getChannelColor(oneNote.channel),
            currentScreen
        );
    }
    render();
    return true;
}

/**
 *
 */
void NoteGround::addNotes(
    const TranslatedNoteSet &notes
) {
    // how long last a screen
    const unsigned durationHalfScreen = microSecondPerPixel * (HEIGHT/2);
    for (const auto oneNote : notes) {


        const unsigned screenStart =
            (oneNote.start + microSecondBeforeStart) /
            durationHalfScreen
        ;
        const unsigned screenEnd =
            (oneNote.end + microSecondBeforeStart) /
            durationHalfScreen
        ;

        for (unsigned i = screenStart; i <= screenEnd; i++) {
            if (notesByHalfScreens.size() <= i) {
                for (unsigned j = notesByHalfScreens.size(); j <= i; j++) {
                    notesByHalfScreens.emplace_back();
                }
            }
            notesByHalfScreens[i].push_back(oneNote);
        }
    }
    // we had one more screen so that we can always do
    // halfScreen[X] + halfScreen[X+1]
    notesByHalfScreens.emplace_back();

}


/**
 *
 */
void NoteGround::addNote(
    const unsigned noteNumber,
    const unsigned start,
    const unsigned end,
    const sf::Color &color,
    const unsigned currentScreen
) {
    if (Keyboard::isOutOfKeyboard(noteNumber)) {
        return;
    }
    const unsigned index = Keyboard::noteToKeyboardIndex(noteNumber);

    const unsigned noteHeight = (end - start) / microSecondPerPixel;
    const unsigned noteYAbsolutePosition = (
        (start + microSecondBeforeStart )/
        microSecondPerPixel
    );
    const int noteYPosition = static_cast<int>(
        noteYAbsolutePosition -
        (currentScreen*(HEIGHT/2))
    );

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
