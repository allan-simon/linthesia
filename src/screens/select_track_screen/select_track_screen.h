#ifndef LINTHESIA_SELECT_TRACK_SCREEN_H
#define LINTHESIA_SELECT_TRACK_SCREEN_H

#include <vector>

#include "screens/abstract_screen.h"
#include "screens/screens.h"

#include "buttons/short_one_line_button.h"

#include "track_box.h"


namespace linthesia {

class SelectTrackScreen : public AbstractScreen {

    public:
        const static ScreenIndex INDEX;

        SelectTrackScreen();

        ScreenIndex run(
            sf::RenderWindow &app,
            Context &context
        );

    protected:
        /**
         * Set the position of the back button on main screen
         */
        void setBackButtonPosition(const sf::RenderWindow &app);
        linthesia::ShortOneLineButton backButton;

        /**
         *
         */
        void setTrackBoxesPosition(const sf::RenderWindow &app);
        std::vector<TrackBox> allTrackBoxes;
        
};


}
#endif

