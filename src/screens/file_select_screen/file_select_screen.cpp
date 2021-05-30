#include <iostream>
#include <algorithm>

#include "utilities/tinydir.h"
#include "utilities/util.h"
#include "file_select_screen.h"

#include "context/context.h"

namespace linthesia {

/**
 *
 */
const ScreenIndex FileSelectScreen::INDEX = "file_select_screen";

/**
 *
 */
const static std::string MIDI_DIR_PATH(DATADIR"/songs/");
const static std::string LOCAL_MIDI_DIR_PATH("../songs/");


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
FileSelectScreen::FileSelectScreen() :
    AbstractScreen(),
    backButton("back")
{

}

/**
 *
 */
ScreenIndex FileSelectScreen::run(
    sf::RenderWindow &app,
    Context &context
) {
    sf::Event event;

    // we initalize the file selector list
    std::string actual_midi_dir_path;
    if (dirExists(MIDI_DIR_PATH.c_str()))
    {
        actual_midi_dir_path = MIDI_DIR_PATH;
    }
    else
    {
        actual_midi_dir_path = LOCAL_MIDI_DIR_PATH;
    }

    midiFileNames = get_midi_files(actual_midi_dir_path);
    fileButtons.clear();
    for (auto fileName : midiFileNames) {
        std::replace( fileName.begin(), fileName.end(), '_', ' '); // replace all '_' to ' '
        fileButtons.push_back(LongOneLineButton(fileName));
    }
    updateIterators(app);
    setFileButtonsPosition(app);

    setBackButtonPosition(app);

    // on purpose
    while (true) {
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return START_APPLICATION;
            }

            if (actionTriggeredFileButtons(app, event, context)) {
                return START_APPLICATION;
            }

            if (backButton.actionTriggered(app, event)) {
                return START_APPLICATION;
            }
        }

        app.clear(BACKGROUND_COLOR);
        app.draw(backButton);
        std::for_each(
            firstDisplayedIt,
            lastDisplayedIt,
            [&](const LongOneLineButton& oneFileButton) {
                app.draw(oneFileButton);
            }
        );
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
    const sf::Event &event,
    Context &context
) {
    // we initalize the file selector list
    std::string actual_midi_dir_path;
    if (dirExists(MIDI_DIR_PATH.c_str()))
    {
        actual_midi_dir_path = MIDI_DIR_PATH;
    }
    else
    {
        actual_midi_dir_path = LOCAL_MIDI_DIR_PATH;
    }

    unsigned buttonNumber = 0;
    bool oneFileChosen = false;
    std::for_each(
        firstDisplayedIt,
        lastDisplayedIt,
        [&](LongOneLineButton& oneFileButton) {

            unsigned currentButton = buttonNumber;
            buttonNumber++;

            if (!oneFileButton.actionTriggered(app, event)) {
                return;
            }
            std::string filename = midiFileNames[currentButton + fileIndex];
            if (!context.openMidiFile(actual_midi_dir_path + filename)) {
                return;
            }
            oneFileChosen = true;
        }
    );

    return oneFileChosen;
}

/**
 *
 */
void FileSelectScreen::setBackButtonPosition(const sf::RenderWindow &app) {
    // put the "back" putton at bottom left (with some padding)
    backButton.setPosition(
        PADDING,
        app.getSize().y - (backButton.getGlobalBounds().height + PADDING)
    );
}

} // end namespace linthesia

