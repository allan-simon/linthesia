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
const static string friendly_app_name = STRING("Linthesia " <<
					       LinthesiaVersionString);

const static string error_header1 = "Linthesia detected a";
const static string error_header2 = " problem and must close:\n\n";
const static string error_footer = "\n\nIf you don't think this should have "
  "happened, please\ncontact Oscar (on Linthesia sourceforge site) and\n"
  "describe what you were doing when the problem\noccurred. Thanks.";

int main(int argc, char *argv[]) {
  Gtk::Main main_loop(argc, argv);
  Gtk::GL::init(argc, argv);
  try {
    string filename("");

    UserSetting::Initialize(application_name);

    if (argc > 1)
      filename = string(argv[1]);

    // strip any leading or trailing quotes from the filename
    // argument (to match the format returned by the open-file
    // dialog later).
    if (filename.length() > 0 &&
	filename[0] == '\"')
      filename = filename.substr(1, filename.length() - 1);

    if (filename.length() > 0 &&
	filename[filename.length()-1] == '\"')
      filename = filename.substr(0, filename.length() - 1);

    Midi *midi = 0;

    // attempt to open the midi file given on the command line first
    if (filename != "") {
      try {
	midi = new Midi(Midi::ReadFromFile(filename));
      }

      catch (const MidiError &e) {
	string wrapped_description = STRING("Problem while loading file: " <<
					    filename <<
					    "\n") + e.GetErrorDescription();
	Compatible::ShowError(wrapped_description);

	filename = "";
	midi = 0;
      }
    }

    // if midi couldn't be opened from command line filename or there
    // simply was no command line filename, use a "file open" dialog.
    if (filename == "") {
      while (!midi) {
	string file_title;
	FileSelector::RequestMidiFilename(&filename, &file_title);

	if (filename != "") {
	  try {
	    midi = new Midi(Midi::ReadFromFile(filename));
	  }
	  catch (const MidiError &e) {
	    string wrapped_description = \
	      STRING("Problem while loading file: " <<
		     file_title <<
		     "\n") + e.GetErrorDescription();
	    Compatible::ShowError(wrapped_description);

	    midi = 0;
	  }
	}

	else {
	  // they pressed cancel, so they must not want to run
	  // the app anymore.
	  return 0;
	}
      }
    }

    Glib::RefPtr<Gdk::GL::Config> glconfig;

    // try double-buffered visual
    glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB    |
        			       Gdk::GL::MODE_DEPTH  |
        			       Gdk::GL::MODE_DOUBLE);
    if (!glconfig) {
      cerr << "*** Cannot find the double-buffered visual.\n"
           << "*** Trying single-buffered visual.\n";

      // try single-buffered visual
      glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                         Gdk::GL::MODE_DEPTH);
      if (!glconfig) {
	string description = STRING(error_header1 <<
				    " OpenGL" <<
				    error_header2 <<
				    "Cannot find any OpenGL-capable visual." <<
				    error_footer);
	Compatible::ShowError(description);
	return 1;
      }
    }

    Gtk::Window window;
    DrawingArea da(glconfig);
    window.add(da);
    window.show_all();

    // do this after gl context is created (ie. after da realized)
    SharedState state;
    state.song_title = FileSelector::TrimFilename(filename);
    state.midi = midi;
    da.init_state_manager(
        state
    );
    




    window.fullscreen();
    window.set_title(friendly_app_name);

    window.set_icon_from_file(string(GRAPHDIR) + "/app_icon.ico");

    // get refresh rate from user settings
    string key = "refresh_rate";
    int rate = 65;
    string user_rate = UserSetting::Get(key, "");
    if (user_rate.empty()) {
      user_rate = STRING(rate);
      UserSetting::Set(key, user_rate);
    }

    else {
      istringstream iss(user_rate);
      if (not (iss >> rate)) {
        Compatible::ShowError("Invalid setting for '"+ key +"' key.\n\nIt will be reset to default value.");
        UserSetting::Set(key, "");
      }
    }

    Glib::signal_timeout().connect(sigc::mem_fun(da, &DrawingArea::GameLoop), 1000/rate);

    main_loop.run(window);

    return 0;
  }

  catch (const LinthesiaError &e) {
    string wrapped_description = STRING(error_header1 <<
					error_header2 <<
					e.GetErrorDescription() <<
					error_footer);
    Compatible::ShowError(wrapped_description);
  }

  catch (const MidiError &e) {
    string wrapped_description = STRING(error_header1 <<
					" MIDI" <<
					error_header2 <<
					e.GetErrorDescription() <<
					error_footer);
    Compatible::ShowError(wrapped_description);
  }

  catch (const exception &e) {
    string wrapped_description = STRING("Linthesia detected an unknown "
					"problem and must close!  '" <<
					e.what() << "'" << error_footer);
    Compatible::ShowError(wrapped_description);
  }

  catch (...) {
    string wrapped_description = STRING("Linthesia detected an unknown "
					"problem and must close!" <<
					error_footer);
    Compatible::ShowError(wrapped_description);
  }

  return 1;
}

