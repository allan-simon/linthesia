#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "score_display.h"

namespace linthesia {

/**
 *
 */
static std::string padZero(unsigned number) {

    std::ostringstream ss;
    ss << std::setw(4) << std::setfill('0') << number;
    return ss.str(); 
}


/**
 *
 */
void ScoreDisplay::init() {

    if (!font.loadFromFile("../fonts/FaunaOne-Regular.ttf")) {
        std::cerr << "Can't load font FaunaOne-Regular" << std::endl;
    }

    unsigned SIZE_COUNTER = 100;
    unsigned x = 1;
    unsigned y = 1;
    // ~yellow
    initLabel(
        perfectLabel,
        sf::Color(200, 200, 50),
        x,
        y
    ); 
    // ~ green
    x += SIZE_COUNTER;
    initLabel(
        excellentLabel,
        sf::Color(50, 200, 50),
        x,
        y
    );

    // ~ green
    x += SIZE_COUNTER;
    initLabel(
        goodLabel,
        sf::Color(50, 200, 50),
        x,
        y
    );
    // ~ green
    x += SIZE_COUNTER;
    initLabel(
        sosoLabel,
        sf::Color(50, 200, 50),
        x,
        y
    ); 
    // ~red
    x += SIZE_COUNTER;
    initLabel(
        missedLabel,
        sf::Color(200, 50, 50),
        x,
        y
    );

}

/**
 *
 */
void ScoreDisplay::initLabel(
    sf::Text &label,
    sf::Color color,
    unsigned x,
    unsigned y
) {

    label.setColor(color);
    label.setFont(font);
    label.setPosition(x, y);

}

/**
 *
 */
void ScoreDisplay::update(const Score &score) {
    missedLabel.setString(padZero(score.missed));
    sosoLabel.setString(padZero(score.soso));
    goodLabel.setString(padZero(score.good));
    excellentLabel.setString(padZero(score.excellent));
    perfectLabel.setString(padZero(score.perfect));
}

/**
 *
 */
void ScoreDisplay::draw(
    sf::RenderTarget& target,
    sf::RenderStates states
) const {
    states.transform *= getTransform();
    target.draw(missedLabel, states);
    target.draw(sosoLabel, states);
    target.draw(goodLabel, states);
    target.draw(excellentLabel, states);
    target.draw(perfectLabel, states);

}

}
