noinst_LTLIBRARIES = libmidi.la
libmidi_la_SOURCES = libmidi/midi.cpp libmidi/midi_util.cpp libmidi/midi_track.cpp libmidi/midi_event.cpp

GRAPHDIR = $(top_srcdir)/graphics

bin_PROGRAMS = linthesia
linthesia_SOURCES = main.cpp \
	screens/screens.cpp \
	screens/main_screen/main_screen.cpp \
	screens/game_over_screen/game_over_screen.cpp \
	screens/one_player_screen/one_player_screen.cpp \
	screens/file_select_screen/file_select_screen.cpp \
	screens/select_track_screen/select_track_screen.cpp

linthesia_LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
linthesia_LDADD = libmidi.la
linthesia_CXXFLAGS = -Wall -std=c++11 -iquote $(top_srcdir)/libmidi
linthesia_CPPFLAGS = -DGRAPHDIR="\"$(GRAPHDIR)\""
