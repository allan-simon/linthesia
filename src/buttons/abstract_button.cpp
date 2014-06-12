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

    if (!font.loadFromFile("../fonts/FaunaOne-Regular.ttf")) {
        std::cerr << "Can't load font FaunaOne-Regular" << std::endl;
    }
    label.setFont(font);
    label.setString(_text);
    //TODO replace this by a smartly calculated position
    //in order to have text centered
    label.setPosition(10.0f,5.0f);

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
    target.draw(label, states);
}

}
