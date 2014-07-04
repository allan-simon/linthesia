#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "black_key.h"

namespace linthesia {

const static int BLACK_KEY_WIDTH = 12;
const static int BLACK_KEY_HEIGHT = 67;


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
void BlackKey::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(blackBody, states);
}


} // end of namespace
