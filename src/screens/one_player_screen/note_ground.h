#ifndef LINTHESIA_NOTE_GROUND
#define LINTHESIA_NOTE_GROUND

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "libmidi/midi.h"
#include "context/context.h"

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
        const static unsigned NOTE_SEPARATOR_WIDTH = 1;


        /**
         *
         */
        sf::FloatRect getGlobalBounds() const;

        /**
         *
         */
        void setMicrosecondBeforeStart(
            const unsigned _microsecondBeforeStart
        );

        /**
         *
         */
        void setSizeFromDurationAndKeyboard(
            const unsigned duration,
            const unsigned keyboardNbrKeys,
            const unsigned microSecondPerPixel
        );

        /**
         *
         */
        void addNotes(
            const TranslatedNoteSet &notes,
            const Context &context
        );

        /**
         * Dump the intermediate renderer into a sprite, that can be used
         * after to be drawn on the window, this function must be called
         * at least once, if you want to see something
         * it should be safe to call it several time
         */
        void render();

    private:
        unsigned microSecondPerPixel;

        unsigned microSecondBeforeStart = 0;

        sf::RectangleShape groundBackground;
        sf::RectangleShape noteSeparator;
        sf::RenderTexture ground;
        sf::Sprite sprite;

        /**
         * add a note to the ground itself
         * @param note number (do, re, mi etc. and octavte)
         * @param note starting time
         * @param note ending time
         * @param color
         */
        void addNote(
            const unsigned noteNumber,
            const unsigned start,
            const unsigned end,
            const sf::Color &color
        );


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

