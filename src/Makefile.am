noinst_LTLIBRARIES = libmidi.la
libmidi_la_SOURCES = libmidi/midi.cpp libmidi/midi_util.cpp libmidi/midi_track.cpp libmidi/midi_event.cpp

GRAPHDIR = $(top_srcdir)/graphics

bin_PROGRAMS = linthesia
linthesia_SOURCES = main.cpp
linthesia_LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
linthesia_LDADD = libmidi.la
linthesia_CXXFLAGS = -Wall -std=c++11 -iquote $(top_srcdir)/libmidi
linthesia_CPPFLAGS = -DGRAPHDIR="\"$(GRAPHDIR)\""
