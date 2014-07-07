#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "trail.h"

namespace linthesia {
/**
 *
 */
KeyboardTrail::KeyboardTrail(
    const int width
) {
    //TODO: replace hardcoded strings by something better
    if (!texture.loadFromFile("../graphics/keyboard_top_separator.png")) {
        std::cerr << "Can't load keyboard_top_separator.png" << std::endl;
    }
    texture.setSmooth(true);
    texture.setRepeated(true);

    sprite.setTextureRect(sf::IntRect(
        0,
        0,
        width,
        static_cast<int>(texture.getSize().y)
    ));
    sprite.setTexture(texture);
}

/**
 *
 */
const sf::FloatRect KeyboardTrail::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

/**
 *
 */
void KeyboardTrail::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}

} // end of namespace

