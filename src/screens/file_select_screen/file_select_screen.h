#ifndef LINTHESIA_FILE_SELECT_SCREEN_H
#define LINTHESIA_FILE_SELECT_SCREEN_H

#include "screens/abstract_screen.h"
#include "screens/screens.h"

#include "buttons/long_one_line_button.h"
#include "buttons/short_one_line_button.h"

namespace linthesia {

class FileSelectScreen : public AbstractScreen {

    public:
        FileSelectScreen();
        const static ScreenIndex INDEX;
        ScreenIndex run(
            sf::RenderWindow &app,
            Context &context
        );

    private:

        /**
         * Check if there's some action to trigger on file buttons
         * and update their internal status (mouse over it etc.)
         * and return true if one button was clicked
         * 
         */
        bool actionTriggeredFileButtons(
            const sf::RenderWindow& app,
            Context &context
        );

        /**
         *
         */
        void setFileButtonsPosition(const sf::RenderWindow& app);
        std::vector<LongOneLineButton> fileButtons;
        std::vector<std::string> midiFileNames;


        /**
         * Iterator on first file button to be displayed
         */
        std::vector<LongOneLineButton>::iterator firstDisplayedIt;

        /**
         * Iterator on last file button to be displayed
         * in order to display just enough to fit in the window
         * and being sure to not go out of bound
         */
        std::vector<LongOneLineButton>::iterator lastDisplayedIt;

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

        /**
         * Set the position of the back button to main screen
         */
        void setBackButtonPosition(const sf::RenderWindow &app);
        linthesia::ShortOneLineButton backButton;


};


}
#endif

