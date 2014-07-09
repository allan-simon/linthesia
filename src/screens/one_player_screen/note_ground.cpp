#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "note_ground.h"

namespace linthesia {

/**
 *
 */
NoteGround::NoteGround() {
    ground.setFillColor(sf::Color(95, 95, 95));
}

/**
 *
 */
sf::FloatRect NoteGround::getGlobalBounds() const {
    return ground.getGlobalBounds();
}

/**
 *
 */
void NoteGround::setSize(
    const sf::Vector2f &size
) {
    ground.setSize(size);
}

/**
 *
 */
void NoteGround::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(ground, states);
}

}
