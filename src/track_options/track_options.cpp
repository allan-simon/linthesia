#include <string>
#include "track_options.h"

namespace linthesia {

/**
 *
 */
TrackOptions::TrackOptions() {
    style2name[PlayStyle::AUTO_PLAY] = "played automatically";
    style2name[PlayStyle::NOT_PLAYED] = "not played";
    style2name[PlayStyle::PLAYED_BY_PLAYER] = "played manually";
}

/**
 *
 */
const std::string TrackOptions::toPreviousStyle(unsigned trackId) {

    if (trackidStyle.find(trackId) == trackidStyle.end()) {
        trackidStyle[trackId] = PlayStyle::AUTO_PLAY;
    }

    switch (trackidStyle[trackId]) {
        case PlayStyle::AUTO_PLAY:
            trackidStyle[trackId] = NOT_PLAYED;
            break;

        case PlayStyle::PLAYED_BY_PLAYER:
            trackidStyle[trackId] = AUTO_PLAY;
            break;

        case PlayStyle::NOT_PLAYED:
            trackidStyle[trackId] = PLAYED_BY_PLAYER;
            break;
    }

    return style2name[trackidStyle[trackId]];
    
}

/**
 *
 */
const std::string TrackOptions::toNextStyle(unsigned trackId) {

    if (trackidStyle.find(trackId) == trackidStyle.end()) {
        trackidStyle[trackId] = PlayStyle::AUTO_PLAY;
    }

    switch (trackidStyle[trackId]) {
        case PlayStyle::AUTO_PLAY:
            trackidStyle[trackId] = PLAYED_BY_PLAYER;
            break;

        case PlayStyle::PLAYED_BY_PLAYER:
            trackidStyle[trackId] = NOT_PLAYED;
            break;

        case PlayStyle::NOT_PLAYED:
            trackidStyle[trackId] = AUTO_PLAY;
            break;
    }

    return style2name[trackidStyle[trackId]];
}

/**
 *
 */
PlayStyle TrackOptions::getStyle(unsigned trackId) {
    return trackidStyle[trackId];
}

/**
 *
 */
bool TrackOptions::isPlayedByComputer(unsigned trackId) {
    return trackidStyle[trackId] == PlayStyle::AUTO_PLAY;
}


}

