#ifndef LINTHESIA_NOTE_GROUND
#define LINTHESIA_NOTE_GROUND

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

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
        NoteGround(float width, float height);

        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

    private:

        sf::RectangleShape ground;

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

