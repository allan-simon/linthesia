#ifndef LINTHESIA_NOTE_GROUND
#define LINTHESIA_NOTE_GROUND

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace linthesia {

/**
 * Represent the ground on which notes to be played will be
 * displayed, it will be a huge rectangle with the notes
 * sticked on it, and we will give the effect of "notes coming"
 * by moving the camera on this ground
 * The handling of the camera will not be done here
 */
class NoteGround : public sf::Drawable , public sf::Transformable {

    public:
        NoteGround();

        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

        /**
         *
         */
        void setSize(
            const sf::Vector2f &size
        );

    private:

        sf::RectangleShape groundBackground;
        sf::RectangleShape noteSeparator;
        sf::RenderTexture ground;
        sf::Sprite sprite;

        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;


};

}

#endif

