#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "utilities/util.h"
#include "assets/path.h"
#include "short_one_line_button.h"


namespace linthesia {

sf::Texture ShortOneLineButton::texture = sf::Texture();
sf::Font ShortOneLineButton::font = sf::Font();

/**
 *
 */
void ShortOneLineButton::init() {
    std::string actual_graphics_dir_path;
    std::string actual_fonts_dir_path;

    if (dirExists(GRAPHICS_DIR))
    {
        actual_graphics_dir_path = GRAPHICS_DIR;
    }
    else
    {
        actual_graphics_dir_path = LOCAL_GRAPHICS_DIR;
    }
    const std::string BUTTON = actual_graphics_dir_path + "button.png";
    if (!texture.loadFromFile(BUTTON)) {
        std::cerr
            << "Can't load "
            << BUTTON
            << std::endl
        ;
    }
    texture.setSmooth(true);

    if (dirExists(FONTS_DIR))
    {
        actual_fonts_dir_path = FONTS_DIR;
    }
    else
    {
        actual_fonts_dir_path = LOCAL_FONTS_DIR;
    }
    if (!font.loadFromFile(actual_fonts_dir_path + DEFAULT_FONT)) {
        std::cerr
            << "Can't load "
            << actual_fonts_dir_path + DEFAULT_FONT
            << std::endl
        ;
    }

}

/**
 *
 */
const sf::Vector2u ShortOneLineButton::getSize() {
    return texture.getSize();
}

/**
 *
 */
ShortOneLineButton::ShortOneLineButton(
    const std::string &_text
):
    OneLineButton()
{
   sprite.setTexture(texture);
    label.setFont(font);
    label.setString(_text);


    //We center the text inside the button
    label.setPosition(
        (sprite.getLocalBounds().width - label.getLocalBounds().width) / 2.0f,
        // TODO: logically it should be  / 2.0 but then it's not centered etc..
        // need to figure out why
        (sprite.getLocalBounds().height - label.getLocalBounds().height) / 4.0f
    );


}

}


