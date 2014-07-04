#ifndef LINTHESIA_KEYBOARD_H
#define LINTHESIA_KEYBOARD_H

#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "black_key.h"
#include "white_key.h"

namespace linthesia {

class Keyboard : public sf::Drawable , public sf::Transformable {

    public:
        Keyboard();

    private:

        /**
         *
         */
        void draw(
            sf::RenderTarget& target,
            sf::RenderStates states
        ) const;

        std::vector<WhiteKey> whiteKeys;
        std::vector<BlackKey> blackKeys;

};


}

#endif

