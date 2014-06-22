#ifndef LINTHESIA_SELECT_MIDI_OUT_H
#define LINTHESIA_SELECT_MIDI_OUT_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace sf {
    class Time;
    class Event;
}

namespace linthesia {

class Context;

/**
 * Represent the button to select the midi output
 * with a button to play as to test
 */
class SelectMidiOut : public sf::Drawable , public sf::Transformable {

    public:
        SelectMidiOut();

        /**
         * Regarding the current events (mouse position etc.)
         * check if there are actions to be triggered
         */
        bool actionTriggered(
            const sf::Window &app,
            const sf::Event &event,
            linthesia::Context &context
        );

        /**
         * Execute all the "passive" actions directly
         * attached to the selector (example: the 'test'
         * audio after you've clicked on play)
         */
        void run(
            const sf::Window &app,
            linthesia::Context& context,
            const sf::Time &delta
        );

        /**
         * Change the name of the output, displayed on the button
         */
        void setOutputName(const std::string &outputName);

    private:

        /**
         * TODO: move in a dedicated 'icon' class
         */
        bool buttonActionTriggered(
            const sf::Window &app,
            const sf::Event &event,
            sf::Sprite &button
        );

        /**
         * TODO: move in a dedicated 'icon' class
         */
        bool buttonContainsPoint(
            const sf::Sprite &button,
            const sf::Vector2i &point
        ) const;

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
        sf::Sprite next;
        
        /**
         *
         */
        sf::Sprite previous;

        /**
         *
         */
        sf::Sprite play;

        /**
         *
         */
        sf::Texture backgroundTexture;

        /**
         *
         */
        sf::Texture iconsTexture; 

        /**
         *
         */
        sf::Font font;

        /**
         *
         */
        sf::Text titleLabel;

        /**
         *
         */
        sf::Text outputLabel;

        /**
         *
         */
        bool isPlaying = false;

};

}

#endif

