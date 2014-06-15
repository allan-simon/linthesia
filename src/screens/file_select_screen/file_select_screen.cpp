#include <iostream>

#include "utilities/tinydir.h"
#include "file_select_screen.h"


namespace linthesia {

/**
 *
 */
const ScreenIndex FileSelectScreen::INDEX = "select_tracks_creen";

/**
 *
 */
const static auto BACKGROUND_COLOR = sf::Color(64, 64, 64);

/**
 * Check if the first string end with the second one
 */
static bool ends_with(const std::string& str, const std::string& suffix) {
    return str.substr(
        std::max(suffix.length(),  str.length()) - suffix.length()
    ) == suffix;
}

/**
 * Get the list of midi files directly in the given directory
 * return a vector of file names
 * note: we don't check if they are valid MIDI files, only if the
 * MIDI extension is here
 */
static std::vector<std::string> get_midi_files(const std::string& path) {

    const std::string MIDI_EXTENSION(".mid");

    std::vector<std::string> midiFileNames;

    tinydir_dir midiDir;
    tinydir_open_sorted(&midiDir, path.c_str());

    for (unsigned i = 0; i < midiDir.n_files; i++) {
        tinydir_file file;
        tinydir_readfile_n(&midiDir, &file, i);

        if (file.is_dir) {
            continue;
        }
        std::string tmpFileName(file.name);
        std::cout << tmpFileName << std::endl;

        if (!ends_with(tmpFileName, MIDI_EXTENSION)) {
           continue ;
        }
        std::cout << tmpFileName << std::endl;
        midiFileNames.push_back(tmpFileName);
    }

    return midiFileNames;
}

/**
 *
 */
ScreenIndex FileSelectScreen::run(sf::RenderWindow &app) {
    sf::Event event;

    std::cout << INDEX << std::endl;

    std::string MIDI_DIR_PATH("../songs");
    std::vector<std::string> midiFileNames = get_midi_files(MIDI_DIR_PATH);

    fileButtons.clear();
    for (auto fileName : midiFileNames) {
        fileButtons.push_back(OneLineButton(fileName));
    }


    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return START_APPLICATION;
            }
        }

        app.clear(BACKGROUND_COLOR);
        //TODO: for test purpose only
        app.draw(fileButtons[0]);
        app.display();
    }

}

} // end namespace linthesia

