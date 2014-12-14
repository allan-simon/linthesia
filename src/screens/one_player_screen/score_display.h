#ifndef LINTHESIA_SCORE_DISPLAY
#define LINTHESIA_SCORE_DISPLAY

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "score.h"

namespace linthesia {

class ScoreDisplay : public sf::Drawable, public sf::Transformable {

    public:

        /**
         *
         */
        void init();

        /**
         *
         */
        void update(const Score &score);

        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

        /**
         * Get the size of the common texture used for all
         * instance of this class, useful so that we don't need
         * to have an instance ready to prepare layout etc.
         */
        static const sf::Vector2u getSize();


    private:

        /**
         *
         */
        void initLabel(
            sf::Text &label,
            sf::Color color,
            unsigned x,
            unsigned y
        );

        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;

        /**
         *
         */
        sf::Text missedLabel;
        sf::Text sosoLabel;
        sf::Text goodLabel;
        sf::Text excellentLabel;
        sf::Text perfectLabel;

        /**
         *
         */
        sf::Font font;



};


}

#endif
