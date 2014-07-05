#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "white_key.h"

namespace linthesia {

const static int WHITE_KEY_WIDTH = 20;
const static int WHITE_KEY_HEIGHT = 112;


/**
 *
 */
WhiteKey::WhiteKey():
    whiteBody(sf::Vector2f(WHITE_KEY_WIDTH, WHITE_KEY_HEIGHT)),
    blackLine(sf::Vector2f(1, WHITE_KEY_HEIGHT))
{
    whiteBody.setFillColor(sf::Color::White);
    blackLine.setFillColor(sf::Color::Black);

    // put black line on the right
    blackLine.setPosition(WHITE_KEY_WIDTH - 1, 0);
}

/**
 *
 */
void WhiteKey::pressed(sf::Color color) {
    whiteBody.setFillColor(color);

}

/**
 *
 */
void WhiteKey::released() {
    whiteBody.setFillColor(sf::Color::White);
}



/**
 *
 */
void WhiteKey::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(whiteBody, states);
    target.draw(blackLine, states);

}


} // end of namespace
