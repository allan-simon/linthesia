#ifndef LINTHESIA_ICON_H
#define LINTHESIA_ICON_H


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf {
    class Drawable;
    class Transformable;
    class Event;
}

namespace linthesia {

const static unsigned ICON_WIDTH = 36;
const static unsigned ICON_HEIGHT = 36;

/**
 * code to choose the icon color
 * it follow the order in the InterfaceButtons.tga file
 */
enum IconColor {
    BLUE = 0 * ICON_WIDTH,
    GREEN = 1 * ICON_WIDTH,
    ORANGE = 2 * ICON_WIDTH,
    YELLOW = 3 * ICON_WIDTH,
    PURPLE = 4 * ICON_WIDTH,
    RED = 5 * ICON_WIDTH,
    GREY = 6 * ICON_WIDTH
};

/**
 * code to choose the icon type
 * it follow the order in the InterfaceButtons.tga file
 */
enum IconType {
    PREVIOUS = 0 * ICON_HEIGHT,
    NEXT = 1 * ICON_HEIGHT,
    SQUARRE = 2 * ICON_HEIGHT,
    SOUND = 3 * ICON_HEIGHT,
    PAUSE = 4 * ICON_HEIGHT
};


/**
 * Represent a "icon" button that can be clickable etc.
 * TODO: maybe interface it with "AbstractButton" ? Clickable ?
 */
class Icon : public sf::Drawable , public sf::Transformable {

    public:
        static void init();

        Icon(
            const IconType type,
            const IconColor color
        );

        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;

        /**
         * Check and trigger action if needed
         * @param containerOffset if icon is not attached directly to the
         *        application window, we need to know the container offset
         *        for checking mouse-related events
         */
        bool actionTriggered(
            const sf::Window &app,
            const sf::Event &event,
            const sf::Vector2f &containerOffset
        );

        /**
         * check if given point is contained inside the icon
         * @param containerOffset used to check if "point" 0,0 is not the icon
         *        top left (0,0), for example mouse coordinate
         */
        bool containsPoint(
            const sf::Vector2i &point,
            const sf::Vector2f &containerOffset
        ) const;



    private:
        sf::Sprite sprite;

        static sf::Texture iconsTexture;


};


} //end of namespace
#endif

