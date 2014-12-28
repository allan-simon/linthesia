#include <string>
#include "track_options.h"

namespace linthesia {

/**
 *
 */
TrackOptions::TrackOptions() {
    style2name[PlayStyle::AUTO_PLAY] = "played automatically";
    style2name[PlayStyle::NOT_PLAYED] = "not played but displayed";
    style2name[PlayStyle::PLAYED_BY_PLAYER] = "played manually";
    style2name[PlayStyle::NOT_PLAYED_NOT_DISPLAYED] = "not played and not displayed";
}

/**
 *
 */
 void TrackOptions::setDefault(unsigned trackId) {
    trackidStyle[trackId] = PlayStyle::AUTO_PLAY;
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
            trackidStyle[trackId] = NOT_PLAYED_NOT_DISPLAYED;
            break;

        case PlayStyle::PLAYED_BY_PLAYER:
            trackidStyle[trackId] = AUTO_PLAY;
            break;

        case PlayStyle::NOT_PLAYED:
            trackidStyle[trackId] = PLAYED_BY_PLAYER;
            break;

        case PlayStyle::NOT_PLAYED_NOT_DISPLAYED:
            trackidStyle[trackId] = NOT_PLAYED;
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
            trackidStyle[trackId] = NOT_PLAYED_NOT_DISPLAYED;
            break;

        case PlayStyle::NOT_PLAYED_NOT_DISPLAYED:
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
bool TrackOptions::isPlayedByComputer(unsigned trackId) const {
    //TODO: quick and dirty fix for issue #73
    if (trackidStyle.find(trackId) == trackidStyle.end()) {
        return true;
    }

    return trackidStyle.at(trackId) == PlayStyle::AUTO_PLAY;
}

/**
 *
 */
bool TrackOptions::isDisplayed(unsigned trackId) const {
    //TODO: quick and dirty fix for issue #73
    if (trackidStyle.find(trackId) == trackidStyle.end()) {
        return false;
    }

        std::cout << trackId << style2name.at(trackidStyle.at(trackId)) <<  std::endl;

    return trackidStyle.at(trackId) != PlayStyle::NOT_PLAYED_NOT_DISPLAYED;
}

}

