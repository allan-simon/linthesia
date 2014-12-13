#ifndef LINTHESIA_TRACK_OPTIONS_H
#define LINTHESIA_TRACK_OPTIONS_H

#include <string>
#include <map>

namespace linthesia {


enum PlayStyle {
    AUTO_PLAY, // played by computer
    PLAYED_BY_PLAYER, // supposed to be played by human player
    NOT_PLAYED, // mute
};



/**
 * Store and permit to modify the different game options for each
 * tracks (if track is played/mute/played by player etc.)
 */
class TrackOptions {

    public:
        TrackOptions();

        /**
         *
         */
        void setDefault(unsigned trackId);

        /**
         * Change the play style of given track to the "previous one"
         * (see PlayStyle enum) and return the Human name of this new
         * style
         */
        const std::string toPreviousStyle(unsigned trackId);

        /**
         * Change the play style of given track to the "next one"
         * (see PlayStyle enum) and return the Human name of this new
         * style
         */
        const std::string toNextStyle(unsigned trackId);

        /**
         *
         */
        bool isPlayedByComputer(unsigned trackId) const;

        /**
         *
         */
        PlayStyle getStyle(unsigned trackId);

    private:
        /**
         * store the track style by trackId
         */
        std::map<unsigned, PlayStyle> trackidStyle;

        /**
         * map a style to its human readable name
         */
        std::map<PlayStyle, std::string> style2name;

};

} // end of namespace

#endif
