#ifndef LINTHESIA_NOTE_GROUND
#define LINTHESIA_NOTE_GROUND

#include <vector>

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
        void init(
            const unsigned keyboardNbrKeys,
            const unsigned microSecondPerPixel
        );

        /**
         *
         */
        void addNotes(const TranslatedNoteSet &notes);


        bool moveAndReRenderIfNecessary(
            const unsigned offset,
            const Context& context
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
        unsigned elaspedPixel = 0;
        unsigned keyboardNbrKeys = 0;

        sf::RectangleShape groundBackground;
        sf::RectangleShape noteSeparator;
        sf::RenderTexture ground;
        sf::Sprite sprite;

        /**
         * the structure is organized like that:
         * 0 => vector of all notes in 1st screen
         * 1 => vector of all notes in 2nd screen
         * 2 => vector of all notes in 3rd screen etc.
         * a note can be there several times if it needs to
         * be displayed on several screen (very long note or
         * note that start on a screen and finish on an other)
         */
        std::vector<std::vector<TranslatedNote>> notesByHalfScreens;

        /**
         * add a note to the ground itself
         * @param note number (do, re, mi etc. and octavte)
         * @param note starting time
         * @param note ending time
         * @param color
         * @param screen on which we draw (to calculate offset)
         */
        void addNote(
            const unsigned noteNumber,
            const unsigned start,
            const unsigned end,
            const sf::Color &color,
            const unsigned currentScreen
        );


        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;


        void drawBackground();
};

}

#endif

