#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "assets/path.h"
#include "two_lines_button.h"


namespace linthesia {

/**
 *
 */
TwoLinesButton::TwoLinesButton(
    const std::string &title,
    const std::string &text
):
    AbstractButton()
{
    const unsigned BUTTON_PADDING = 20;
    const unsigned CHARACTER_SIZE = 15;
    const unsigned INTER_LINE_SPACE = 15;
    currentState = ButtonStates::NORMAL;

    const std::string BUTTON = GRAPHICS_DIR "button2line.png";
    if (!texture.loadFromFile(BUTTON)) {
        std::cerr
            << "Can't load "
            << BUTTON
            << std::endl
        ;
    }
    texture.setSmooth(true);
    sprite.setTexture(texture);

    if (!font.loadFromFile(DEFAULT_FONT)) {
        std::cerr
            << "Can't load "
            << DEFAULT_FONT
            << std::endl
        ;
    }

    titleLabel.setFont(font);
    titleLabel.setCharacterSize(CHARACTER_SIZE);
    titleLabel.setStyle(sf::Text::Bold);
    titleLabel.setString(title);
    titleLabel.setPosition(
        BUTTON_PADDING,
        BUTTON_PADDING
    );

    textLabel.setFont(font);
    textLabel.setCharacterSize(CHARACTER_SIZE);
    textLabel.setString(text);
    textLabel.setPosition(
        BUTTON_PADDING,
        BUTTON_PADDING + CHARACTER_SIZE + INTER_LINE_SPACE
    );

}

/**
 *
 */
sf::FloatRect TwoLinesButton::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

/**
 *
 */
void TwoLinesButton::setHovered() {
    const auto HOVERED_COLOR = sf::Color(200, 200, 200);
    if (currentState == ButtonStates::HOVERED) {
        return;
    }
    currentState = ButtonStates::HOVERED;
    sprite.setColor(HOVERED_COLOR);
    titleLabel.setColor(HOVERED_COLOR);
    textLabel.setColor(HOVERED_COLOR);
}

/**
 *
 */
void TwoLinesButton::setUnhovered() {
    if (currentState != ButtonStates::HOVERED) {
        return;
    }
    currentState = ButtonStates::NORMAL;
    sprite.setColor(sf::Color::White);
    titleLabel.setColor(sf::Color::White);
}

/**
 *
 */
 void TwoLinesButton::setText(const std::string &newText) {
    textLabel.setString(newText);
 }

/**
 *
 */
void TwoLinesButton::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
    target.draw(titleLabel, states);
    target.draw(textLabel, states);
}

}
