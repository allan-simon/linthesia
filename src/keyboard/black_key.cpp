#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "black_key.h"

namespace linthesia {

/**
 *
 */
BlackKey::BlackKey():
    blackBody(sf::Vector2f(BLACK_KEY_WIDTH, BLACK_KEY_HEIGHT))
{
    blackBody.setFillColor(sf::Color::Black);

}

/**
 *
 */
void BlackKey::pressed(const sf::Color &color) {
    blackBody.setFillColor(color);

}

/**
 *
 */
void BlackKey::released() {
    blackBody.setFillColor(sf::Color::Black);
}



/**
 *
 */
void BlackKey::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(blackBody, states);
}


} // end of namespace
