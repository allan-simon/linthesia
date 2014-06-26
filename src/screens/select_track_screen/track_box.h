#ifndef LINTHESIA_TRACK_BOX_H
#define LINTHESIA_TRACK_BOX_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "icons/icon.h"

namespace sf {
    class Time;
    class Event;
}

namespace linthesia {

class Context;

class TrackBox : public sf::Drawable , public sf::Transformable {


    public:
        TrackBox(
            const std::string &instrumentName,
            unsigned notesNumber
        );

        /**
         *
         */
        static void init();

        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

        /**
         * Regarding the current events (mouse position etc.)
         * check if there are actions to be triggered
         */
        bool actionTriggered(
            const sf::Window &app,
            const sf::Event &event,
            linthesia::Context &context
        );

    private:
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
        sf::Sprite background;

        /**
         *
         */
        Icon next;
        
        /**
         *
         */
        Icon previous;

        /**
         *
         */
        static sf::Texture backgroundTexture;

        /**
         *
         */
        sf::Text instrumentLabel;

        /**
         *
         */
        sf::Text instrumentNameLabel;

        /**
         *
         */
        sf::Text notesLabel;

        /**
         *
         */
        sf::Text notesNumberLabel;

        /**
         *
         */
        sf::Text playChoiceLabel;

        /**
         *
         */
        static sf::Font font;



};

} // end of namespace
#endif

