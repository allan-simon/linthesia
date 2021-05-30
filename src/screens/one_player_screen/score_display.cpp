#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "utilities/util.h"
#include "assets/path.h"
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
    std::string actual_graphics_dir_path;
    std::string actual_fonts_dir_path;

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

    if (dirExists(GRAPHICS_DIR))
    {
        actual_graphics_dir_path = GRAPHICS_DIR;
    }
    else
    {
        actual_graphics_dir_path = LOCAL_GRAPHICS_DIR;
    }
    const std::string STAR = actual_graphics_dir_path + "star.png";
    if (!star.loadFromFile(STAR)) {
        std::cerr
            << "Can't load "
            << STAR
            << std::endl
        ;
    }

    //star.setSmooth(true);
    starSprite.setTexture(star);

    const std::string THUMB_DOWN = actual_graphics_dir_path + "thumb_down.png";
    if (!thumbdown.loadFromFile(THUMB_DOWN)) {
        std::cerr
            << "Can't load "
            << THUMB_DOWN
            << std::endl
        ;
    }
    //thumbdown.setSmooth(true);
    thumbdownSprite.setTexture(thumbdown);


    const unsigned SIZE_COUNTER = 100;
    const unsigned SIZE_COUNTER_ICON = 40;
    const unsigned ICON_PADDING_TOP = 5;
    unsigned x = 0;
    unsigned y = 0;

    // ~yellow
    starSprite.setPosition(x, ICON_PADDING_TOP);
    x += SIZE_COUNTER_ICON;
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
    thumbdownSprite.setPosition(x, ICON_PADDING_TOP);
    x += SIZE_COUNTER_ICON;
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

    label.setFillColor(color);
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
    target.draw(thumbdownSprite, states);
    target.draw(missedLabel, states);
    target.draw(sosoLabel, states);
    target.draw(goodLabel, states);
    target.draw(excellentLabel, states);
    target.draw(starSprite, states);
    target.draw(perfectLabel, states);

}

}
