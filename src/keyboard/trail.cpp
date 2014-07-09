#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "trail.h"

namespace linthesia {
/**
 *
 */
KeyboardTrail::KeyboardTrail() {
    //TODO: replace hardcoded strings by something better
    if (!texture.loadFromFile("../graphics/keyboard_top_separator.png")) {
        std::cerr << "Can't load keyboard_top_separator.png" << std::endl;
    }
    texture.setSmooth(true);
    texture.setRepeated(true);
    sprite.setTexture(texture);
}

/**
 *
 */
void KeyboardTrail::setWidth(const int width) {
    sprite.setTextureRect(sf::IntRect(
        0,
        0,
        width,
        static_cast<int>(texture.getSize().y)
    ));
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

