#ifndef LINTHESIA_SCREENS_H
#define LINTHESIA_SCREENS_H

#include <map>
#include <memory>
#include "screen_index.h"
#include "abstract_screen.h"

namespace linthesia {

// special index used to express that we're not switching to an other
// screen, but that we are quitting the application
static ScreenIndex STOP_APPLICATION = "stopapplication";

// special index used to represent the first screen displayed when the
// application start 
static ScreenIndex START_APPLICATION = "startapplication";

typedef std::map<
    linthesia::ScreenIndex,
    std::unique_ptr<linthesia::AbstractScreen>
> Screens;

Screens init_game_screens();

}
#endif
