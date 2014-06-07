# -*- mode: makefile-gmake; coding: utf-8 -*-

CXX = clang++
CXXFLAGS = -I . -I .. -ansi -Wall -O2 --std=c++11
TARGET = libmidi.a

all: $(TARGET)

$(TARGET): midi.o midi_util.o midi_track.o midi_event.o
	ar rcs $@ $^

.PHONY:clean
clean:
	find . -name "*.o" -delete
	find . -name "*~" -delete
	$(RM) $(TARGET)
