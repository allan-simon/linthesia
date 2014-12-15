#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "assets/path.h"
#include "icon.h"

namespace linthesia {

sf::Texture Icon::iconsTexture = sf::Texture();

/**
 *
 */
void Icon::init() {
    const std::string INTERFACE_BUTTONS = GRAPHICS_DIR "InterfaceButtons.tga";
    if (!iconsTexture.loadFromFile(INTERFACE_BUTTONS)) {
        std::cerr
            << "Can't load "
            << INTERFACE_BUTTONS
            << std::endl
        ;
    }
    iconsTexture.setSmooth(true);
}

/**
 *
 */
Icon::Icon(
    const IconType type,
    const IconColor color
) :
    sf::Drawable(),
    sf::Transformable()
{

    sprite = sf::Sprite();
    sprite.setTexture(iconsTexture);
    sprite.setTextureRect(sf::IntRect(
        type,
        color,
        ICON_WIDTH,
        ICON_HEIGHT
    ));
}

/**
 *
 */
bool Icon::actionTriggered(
    const sf::Window &app,
    const sf::Event &event,
    const sf::Vector2f &containerOffset
) {

    //TODO: implement "icon" state (hovered / clicked etc.)
    if (!containsPoint(sf::Mouse::getPosition(app), containerOffset)) {
        return false;
    }
    if (
        event.type == sf::Event::MouseButtonPressed &&
        sf::Mouse::isButtonPressed(sf::Mouse::Left)
    ) {
        return true;
    }

    return false;

}

/**
 *
 */
bool Icon::containsPoint(
    const sf::Vector2i &point,
    const sf::Vector2f &containerOffset
) const {
    // we subtract the x,y position of the button itself
    // as the point given in parameter has its coordinate relative to
    // the window
    return sprite.getGlobalBounds().contains(
        point.x - containerOffset.x - getPosition().x,
        point.y - containerOffset.y - getPosition().y
    );
}



/**
 *
 */
void Icon::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}


} // end of namespace

