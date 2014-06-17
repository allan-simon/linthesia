#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "one_line_button.h"


namespace linthesia {

/**
 *
 */
OneLineButton::OneLineButton(
):
    AbstractButton()
{
    currentState = ButtonStates::NORMAL;
}
/**
 *
 */
sf::FloatRect OneLineButton::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

/**
 *
 */
void OneLineButton::setHovered() {
    if (currentState == ButtonStates::HOVERED) {
        return;
    }
    currentState = ButtonStates::HOVERED;
    sprite.setColor(sf::Color(200,200,200));
    label.setColor(sf::Color(200,200,200));
}

/**
 *
 */
void OneLineButton::setUnhovered() {
    if (currentState != ButtonStates::HOVERED) {
        return;
    }
    currentState = ButtonStates::NORMAL;
    sprite.setColor(sf::Color::White);
    label.setColor(sf::Color::White);
}

/**
 *
 */
void OneLineButton::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
    target.draw(label, states);
}

}
