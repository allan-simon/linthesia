#ifndef LINTHESIA_CONTEXT_H
#define LINTHESIA_CONTEXT_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System.hpp>
#include "libmidi/midi.h"
#include "midi_io/midi_out.h"
#include "midi_io/midi_in.h"
#include "track_options/track_options.h"

namespace linthesia {

class Context {
    
    public:
        Context();
        ~Context();

        bool openMidiFile(const std::string& filename);

        MidiEventListWithTrackId update(const sf::Int64 deltaMicroseconds);

        const MidiTrackList &getTracks() const;
        std::string getFilename() const;

        /**
         * Get the complete list of notes in current midi files
         */
        const TranslatedNoteSet &getNotes() const;

        /**
         * get the length of current song
         */
        unsigned getSongLength () const;

        /**
         *
         */
        unsigned getSongPosition () const;

        /**
         * reset song to beginning
         */
        void resetSong();

        /**
         * Get the color used to represent a given channel
         * As we may want later to make it user configurable
         */
        sf::Color getChannelColor(const unsigned channel) const;

        linthesia::MidiOut midiOut;
        linthesia::MidiIn midiIn;
        linthesia::TrackOptions tracksOptions;
    private:
        Midi* midi;
        std::string filename;

        void setDefaultTrackOptions();

};


} // end namespace linthesia
#endif

