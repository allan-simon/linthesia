#ifndef LINTHESIA_FILE_SELECT_SCREEN_H
#define LINTHESIA_FILE_SELECT_SCREEN_H

#include "screens/abstract_screen.h"
#include "screens/screens.h"

#include "buttons/one_line_button.h"

namespace linthesia {

class FileSelectScreen : public AbstractScreen {

    public:
        const static ScreenIndex INDEX;
        ScreenIndex run(sf::RenderWindow &app); 

    private:

        /**
         *
         */
        void setFileButtonsPosition(const sf::RenderWindow& app);
        std::vector<OneLineButton> fileButtons;


        /**
         * Iterator on first file button to be displayed
         */
        std::vector<OneLineButton>::iterator firstDisplayedIt;

        /**
         * Iterator on last file button to be displayed
         * in order to display just enough to fit in the window
         * and being sure to not go out of bound
         */
        std::vector<OneLineButton>::iterator lastDisplayedIt;

        /**
         * Update the first and last file buttons iterator
         * (in case of windows resize, people click on next/previous)
         * files being added etc.
         */
        void updateIterators(const sf::RenderWindow& app);

        /**
         * Index of the first file displayed in the list of files
         * TODO: not sure if really needed, maybe ease comprehension
         */
        unsigned fileIndex = 0;
};


}
#endif

