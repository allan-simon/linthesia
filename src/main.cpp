#include <string>
#include <iostream>

#include <SFML/Window.hpp>

#include "libmidi/midi.h"
#include "libmidi/midi_util.h"

#include "screens/screens.h"


using namespace std;


Midi* get_midi_file_from_cli(std::string& filename);
/**
 *
 */
Midi* get_midi_file_from_cli(std::string& filename) {
    try {
        return new Midi(Midi::read_from_file(filename));

    } catch (const MidiError &e) {
        string wrappedDescription = STRING(
            "Problem while loading file: " <<
            filename <<
            "\n"
        ) + e.get_error_description();

        cerr << wrappedDescription << endl;
        filename = "";
        return nullptr;
    }
}



int main(int argc, char *argv[]) {

    string filename;
    if (argc != 2) {
        cerr << "For the time being, you must run linthesia with" <<
            " a filename" <<
            endl;
        return 1;
    }

    filename = string(argv[1]);
    Midi* midi = get_midi_file_from_cli(filename);

    sf::RenderWindow application(
        sf::VideoMode(800, 600),
        "Linthesia"
    );

    auto screens = linthesia::init_game_screens();
    auto currentScreen = linthesia::START_APPLICATION;

    while (currentScreen != linthesia::STOP_APPLICATION) {
        currentScreen = screens[currentScreen]->run(application);
    }

    delete midi;

    return 0;
}


