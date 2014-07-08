#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "note_ground.h"

namespace linthesia {

/**
 *
 */
NoteGround::NoteGround(float width, float height):
    ground(sf::Vector2f(width, height))
{
    ground.setFillColor(sf::Color(223, 223, 223));
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
void NoteGround::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(ground, states);
}

}
