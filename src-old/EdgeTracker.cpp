#include "EdgeTracker.h"

EdgeTracker::EdgeTracker() : 
    active(true),
    justActive(true) {
}

void EdgeTracker::activate() {
    justActive = true;
    active = true;
}

void EdgeTracker::deactivate() {
    justActive = false;
    active = false;
}

bool EdgeTracker::is_active() {
    return active;
}

bool EdgeTracker::is_not_active() {
    return !active;
}


bool EdgeTracker::just_activated() {
    bool was_active = justActive;
    justActive = false;
    return was_active;
}


