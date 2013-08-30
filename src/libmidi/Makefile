# -*- mode: makefile-gmake; coding: utf-8 -*-

CXX = g++
CXXFLAGS = -I . -I .. -ansi -Wall -O2
TARGET = libmidi.a

all: $(TARGET)

$(TARGET): Midi.o MidiUtil.o MidiTrack.o MidiEvent.o
	ar rcs $@ $^

.PHONY:clean
clean:
	find . -name "*.o" -delete
	find . -name "*~" -delete
	$(RM) $(TARGET)
