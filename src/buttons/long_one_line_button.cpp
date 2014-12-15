#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "assets/path.h"
#include "long_one_line_button.h"


namespace linthesia {

sf::Texture LongOneLineButton::texture = sf::Texture();
sf::Font LongOneLineButton::font = sf::Font();

/**
 *
 */
void LongOneLineButton::init() {

    const std::string BUTTON = GRAPHICS_DIR "longbutton.png";
    if (!texture.loadFromFile(BUTTON)) {
        std::cerr
            << "Can't load "
            << BUTTON
            << std::endl
        ;
    }
    texture.setSmooth(true);

    if (!font.loadFromFile(DEFAULT_FONT)) {
        std::cerr
            << "Can't load "
            << DEFAULT_FONT
            << std::endl
        ;
    }

}

/**
 *
 */
const sf::Vector2u LongOneLineButton::getSize() {
    return texture.getSize();
}

/**
 *
 */
LongOneLineButton::LongOneLineButton(
    const std::string &_text
):
    OneLineButton()
{
    //TODO: manually calculated for the text to not go
    // over the button texture if using latin character
    // replace that by something smarter based on final
    // text size in pixel and texture size
    const unsigned MAX_TEXT_SIZE = 25;

    const std::string ELLISPIS = "...";
    const unsigned PADDING = 10;
    sprite.setTexture(texture);
    label.setFont(font);

    // if the text is too long we will cut it and put "..."
    if (_text.size() > MAX_TEXT_SIZE) {
        label.setString(_text.substr(0, MAX_TEXT_SIZE) + ELLISPIS);
    } else {
        label.setString(_text);
    }



    //We put the text on the left and vertically centered
    label.setPosition(
        PADDING,
        // TODO: logically it should be  / 2.0 but then it's not centered etc..
        // need to figure out why
        (sprite.getLocalBounds().height - label.getLocalBounds().height) / 4.0f
    );

}

}

