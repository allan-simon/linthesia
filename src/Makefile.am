noinst_LTLIBRARIES = libmidi.la
libmidi_la_SOURCES = libmidi/midi.cpp libmidi/midi_util.cpp libmidi/midi_track.cpp libmidi/midi_event.cpp

GRAPHDIR = $(top_srcdir)/graphics

bin_PROGRAMS = linthesia
linthesia_SOURCES = main.cpp \
	buttons/abstract_button.cpp \
	buttons/one_line_button.cpp \
	buttons/short_one_line_button.cpp \
	buttons/long_one_line_button.cpp \
	buttons/two_lines_button.cpp \
	context/context.cpp \
	midi_io/midi_out.cpp \
	midi_io/midi_in.cpp \
	icons/icon.cpp \
	screens/screens.cpp \
	screens/main_screen/main_screen.cpp \
	screens/main_screen/select_midi_out.cpp\
	screens/main_screen/select_midi_in.cpp\
	screens/game_over_screen/game_over_screen.cpp \
	screens/one_player_screen/one_player_screen.cpp \
	screens/file_select_screen/file_select_screen.cpp \
	screens/select_track_screen/track_box.cpp \
	screens/select_track_screen/select_track_screen.cpp

linthesia_DEPENDENCIES = libmidi.la

linthesia_LDFLAGS = -lrtmidi

linthesia_LDADD = $(linthesia_DEPENDENCIES) $(RTMIDI_LIBS)
linthesia_LDADD += -lsfml-graphics -lsfml-window -lsfml-system

linthesia_CXXFLAGS = -Wall -std=c++11 -iquote $(top_srcdir)/libmidi

linthesia_CPPFLAGS = -DGRAPHDIR="\"$(GRAPHDIR)\""
linthesia_CPPFLAGS = $(RTMIDI_CFLAGS)
