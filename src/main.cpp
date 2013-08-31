// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#include <string>

#include "OSGraphics.h"
#include "StringUtil.h"
#include "FileSelector.h"
#include "UserSettings.h"
#include "Version.h"
#include "CompatibleSystem.h"
#include "LinthesiaError.h"
#include "SharedState.h"

#include "DrawingArea.h"

#include "libmidi/Midi.h"
#include "libmidi/MidiUtil.h"

#ifndef GRAPHDIR
#define GRAPHDIR "../graphics"
#endif

using namespace std;


const static string application_name = "Linthesia";
const static string friendly_app_name = "Linthesia " + LinthesiaVersionString;

const static string error_header1 = "Linthesia detected a";
const static string error_header2 = " problem and must close:\n\n";
const static string error_footer = "\n\nIf you don't think this should have "
  "happened, please\ncontact Oscar (on Linthesia sourceforge site) and\n"
  "describe what you were doing when the problem\noccurred. Thanks.";


/**
 *
 */
string clean_file_name(string filename) {
   if (
        filename.length() > 0 &&
        filename[0] == '\"'
    ) {
        filename = filename.substr(1, filename.length() - 1);
    }

    // strip any leading or trailing quotes from the filename
    // argument (to match the format returned by the open-file
    // dialog later).
     if (
        filename.length() > 0 &&
	    filename[filename.length()-1] == '\"'
    ) {
      filename = filename.substr(0, filename.length() - 1);
    }
    return filename;
};

/**
 *
 */
Midi* get_midi_file_from_cli(std::string& filename) {
    try {
        return new Midi(Midi::ReadFromFile(filename));

    } catch (const MidiError &e) {
        string wrapped_description = STRING("Problem while loading file: " <<
                filename <<
                "\n") + e.GetErrorDescription();
        Compatible::ShowError(wrapped_description);
        filename = "";
        return nullptr;
    }
}


/**
 *
 */
// if midi couldn't be opened from command line filename or there
// simply was no command line filename, use a "file open" dialog.
Midi* get_midi_from_file_selector(std::string& filename) {
    Midi *midi = nullptr;
    while (!midi) {
        string file_title;
        FileSelector::RequestMidiFilename(&filename, &file_title);
        if (filename == "") {
            // they pressed cancel, so they must not want to run
            // the app anymore.
            return nullptr;
        }

        try {
            midi = new Midi(Midi::ReadFromFile(filename));
        } catch (const MidiError &e) {
            string wrapped_description = "Problem while loading file: " +
                file_title +
                "\n" +
                e.GetErrorDescription();
            Compatible::ShowError(wrapped_description);
            midi = nullptr;
        }
    }

    return midi;
}




/**
 *
 */
Midi* get_midi_file(std::string& filename) {
    Midi *midi = nullptr;
    filename = clean_file_name(filename);
    // attempt to open the midi file given on the command line first
    if (filename != "") {
        midi = get_midi_file_from_cli(filename);
    }

    if (midi == nullptr) {
        midi = get_midi_from_file_selector(filename);
    }

    return midi;

}

/**
 *
 */
Glib::RefPtr<Gdk::GL::Config> get_glconfig() {
    // try double-buffered visual
     Glib::RefPtr<Gdk::GL::Config> glconfig = Gdk::GL::Config::create(
        Gdk::GL::MODE_RGB    |
        Gdk::GL::MODE_DEPTH  |
        Gdk::GL::MODE_DOUBLE
    );

    if (glconfig) {
        return glconfig;
    }
    cerr << "*** Cannot find the double-buffered visual.\n"
        << "*** Trying single-buffered visual.\n";

    // try single-buffered visual
    glconfig = Gdk::GL::Config::create(
        Gdk::GL::MODE_RGB |
        Gdk::GL::MODE_DEPTH
    );
    if (!glconfig) {
        return glconfig; 
    }
    string description = error_header1 +
        " OpenGL" +
        error_header2 +
        "Cannot find any OpenGL-capable visual." +
        error_footer;
    Compatible::ShowError(description);
    return glconfig;


}

int get_refresh_rate() {
    string key = "refresh_rate";
    int rate = 65;
    string user_rate = UserSetting::Get(key, "");
    if (user_rate.empty()) {

        user_rate = STRING(rate);
        UserSetting::Set(key, user_rate);

    } else {
        istringstream iss(user_rate);
        if (not (iss >> rate)) {
            Compatible::ShowError(
                "Invalid setting for '"+ key +"' key.\n\n"
                "Reset to default value when reload."
            );
            UserSetting::Set(key, "");
        }
    }
    return rate; 
}




void setup_game_window(
    Gtk::Window &window,
    DrawingArea &drawingArea
) {
    
   
    window.add(drawingArea);
    window.show_all();

    window.fullscreen();
    window.set_title(friendly_app_name);

    window.set_icon_from_file(string(GRAPHDIR) + "/app_icon.ico");
 
    // get refresh rate from user settings
    int rate = get_refresh_rate();

    Glib::signal_timeout().connect(
        sigc::mem_fun(
            drawingArea,
            &DrawingArea::GameLoop
        ),
        1000/rate
    );
}


int main(int argc, char *argv[]) {
    Gtk::Main main_loop(argc, argv);
    Gtk::GL::init(argc, argv);

    string filename;
    if (argc > 1) {
        filename = string(argv[1]);
    }


    try {
        UserSetting::Initialize(application_name);

        Midi *midi = get_midi_file(filename);
        if (midi == nullptr) {
            return 0;
        }

        Glib::RefPtr<Gdk::GL::Config> glconfig = get_glconfig();
        if (!glconfig) {
            return 1;
        }


        Gtk::Window window;
        DrawingArea da(glconfig);
        // do this after gl context is created (ie. after da realized)
        setup_game_window(window, da);
        
        SharedState state;
        state.song_title = FileSelector::TrimFilename(filename);
        state.midi = midi;
        da.init_state_manager(
            state
        );
        

        main_loop.run(window);

        return 0;

    } catch (const LinthesiaError &e) {
        string wrapped_description = error_header1 +
            error_header2 +
            e.GetErrorDescription() +
            error_footer;
        Compatible::ShowError(wrapped_description);

    } catch (const MidiError &e) {
        string wrapped_description = error_header1 +
            " MIDI" +
            error_header2 +
            e.GetErrorDescription() +
            error_footer;
        Compatible::ShowError(wrapped_description);

    } catch (const exception &e) {
        string wrapped_description = string("Linthesia detected an unknown ") +
            "problem and must close!  '" +
            e.what() +
            "'" +
            error_footer;
        Compatible::ShowError(wrapped_description);

    } catch (...) {
        string wrapped_description = string("Linthesia detected an unknown ") +
            "problem and must close!" +
            error_footer;
        Compatible::ShowError(wrapped_description);
    }

    return 1;
}

