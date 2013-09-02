noinst_LTLIBRARIES = libmidi.la
libmidi_la_SOURCES = libmidi/Midi.cpp libmidi/MidiUtil.cpp libmidi/MidiTrack.cpp libmidi/MidiEvent.cpp

GRAPHDIR = $(top_srcdir)/graphics

bin_PROGRAMS = linthesia
linthesia_SOURCES = CompatibleSystem.cpp EdgeTracker.cpp KeyboardDisplay.cpp MenuLayout.cpp Renderer.cpp TextWriter.cpp TrackSelectionState.cpp DeviceTile.cpp FileSelector.cpp LinthesiaError.cpp MidiComm.cpp StatsState.cpp Tga.cpp TrackTile.cpp DrawingArea.cpp GameState.cpp main.cpp PlayingState.cpp StringTile.cpp TitleState.cpp UserSettings.cpp
linthesia_LDFLAGS = @GTKMM_LIBS@ @GCONFMM_LIBS@ @GTKGLEXTMM_LIBS@ @ALSA_LIBS@
linthesia_LDADD = libmidi.la
linthesia_CXXFLAGS = -std=c++11 -iquote $(top_srcdir)/libmidi
linthesia_CPPFLAGS = @GTKMM_CFLAGS@ @GCONFMM_CFLAGS@ @GTKGLEXTMM_CFLAGS@ @ALSA_CFLAGS@ -DGRAPHDIR="\"$(GRAPHDIR)\""
