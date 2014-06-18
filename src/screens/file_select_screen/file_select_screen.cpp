#include <iostream>
#include <algorithm>

#include "utilities/tinydir.h"
#include "file_select_screen.h"

#include "context/context.h"

namespace linthesia {

/**
 *
 */
const ScreenIndex FileSelectScreen::INDEX = "select_tracks_creen";

/**
 *
 */
const static std::string MIDI_DIR_PATH("../songs/");

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
    tinydir_close(&midiDir);
    return midiFileNames;
}

/**
 *
 */
ScreenIndex FileSelectScreen::run(
    sf::RenderWindow &app,
    Context &context
) {
    sf::Event event;

    std::cout << INDEX << std::endl;

    midiFileNames = get_midi_files(MIDI_DIR_PATH);

    fileButtons.clear();
    for (auto fileName : midiFileNames) {
        fileButtons.push_back(LongOneLineButton(fileName));
    }
    updateIterators(app);
    setFileButtonsPosition(app);


    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return START_APPLICATION;
            }

            if (actionTriggeredFileButtons(app, context)) {
                return START_APPLICATION;
            }
        }

        app.clear(BACKGROUND_COLOR);
        std::for_each(
            firstDisplayedIt,
            lastDisplayedIt,
            [&](const LongOneLineButton& oneFileButton) {
                app.draw(oneFileButton);
            }
        );
        //TODO: for test purpose only
        app.display();
    }

}

/**
 *
 */
void FileSelectScreen::updateIterators(
    const sf::RenderWindow& app
) {
    unsigned windowHeight = app.getSize().y;
    unsigned maxDisplayableFileButton = 0;

    // we first calculate how many file button fit in the
    // visual place available
    maxDisplayableFileButton = windowHeight / LongOneLineButton::getSize().y;

    firstDisplayedIt = fileButtons.begin() + fileIndex;

    // the last displayed item is either the
    // "fileIndex + maxDisplayableFileButton"th fileButton
    // or the actual last one
    if (fileIndex + maxDisplayableFileButton < fileButtons.size()) {
        lastDisplayedIt = firstDisplayedIt + maxDisplayableFileButton;
    } else {
        lastDisplayedIt = fileButtons.end();
    }
}

/**
 *
 */
void FileSelectScreen::setFileButtonsPosition(const sf::RenderWindow& app) {

    // fileButtons are centered, as they all have the same size
    // we compute it only once
    unsigned widthCenter = (app.getSize().x - LongOneLineButton::getSize().x) / 2;

    unsigned i = 0;
    std::for_each(
        firstDisplayedIt,
        lastDisplayedIt,

        // [&] in order to have access to i from inside the lambda
        [&](LongOneLineButton& oneFileButton) {
            //TODO : maybe add some margin before each file button
            // right now they are stick each after the other
            i += LongOneLineButton::getSize().y;
            oneFileButton.setPosition(
                widthCenter,
                i
            );
        }

    );

}

/**
 *
 */
bool FileSelectScreen::actionTriggeredFileButtons(
    const sf::RenderWindow &app,
    Context &context
) {

    unsigned buttonNumber = 0;
    bool oneFileChosen = false;
    std::for_each(
        firstDisplayedIt,
        lastDisplayedIt,
        [&](LongOneLineButton& oneFileButton) {
            buttonNumber++;
            if (!oneFileButton.actionTriggered(app)) {
                return;
            }
            std::string filename = midiFileNames[buttonNumber+fileIndex];
            if (!context.openMidiFile(MIDI_DIR_PATH + filename)) {
                return;
            }
            oneFileChosen = true;

        }
    );

    return oneFileChosen;
}

} // end namespace linthesia

