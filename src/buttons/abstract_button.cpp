
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "abstract_button.h"


namespace linthesia {


/**
 *
 */
bool AbstractButton::containsPoint(sf::Vector2i point) const {
    // we subtract the x,y position of the button itself
    // as the point given in parameter has its coordinate relative to
    // the window
    return getGlobalBounds().contains(
        point.x - getPosition().x,
        point.y - getPosition().y
    );
}

/**
 *
 */
bool AbstractButton::actionTriggered(
    const sf::Window &app,
    const sf::Event &event
) {
    // disabled button are not clickable / hoverable etc.
    if (currentState == ButtonStates::DISABLED) {
        return false;
    }

    if (!containsPoint(sf::Mouse::getPosition(app))) {
        setUnhovered();
        return false;
    }

    setHovered();
    if (
        event.type == sf::Event::MouseButtonPressed &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left)
    ) {
        // only if we left click inside the button
        // we will consider the action of this button
        // triggered
        return true;
    }

    return false;
}




}
