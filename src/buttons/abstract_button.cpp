#include <iostream>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "abstract_button.h"


namespace linthesia {

/**
 *
 */
AbstractButton::AbstractButton(
    const std::string &_text
) {

    //TODO: replace hardcoded strings by something better
    if (!texture.loadFromFile("../graphics/button.png")) {
        std::cerr << "Can't load button.png" << std::endl;
    }
    sprite.setTexture(texture);

    //TODO: display text on top of the button
}

/**
 *
 */
bool AbstractButton::containsPoint(sf::Vector2i point) const {
    return sprite.getGlobalBounds().contains(point.x, point.y);
}

/**
 *
 */
void AbstractButton::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}

}
