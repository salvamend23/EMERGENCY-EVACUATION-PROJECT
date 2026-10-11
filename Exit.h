#pragma once
#include "Location.h"

//represents an emergency exit in the building
class Exit : public Location {
private:
    bool available;

public:
    Exit(int exitId, const std::string& exitName, bool isAvailable = true)
        : Location(exitId, exitName), available(isAvailable) {
    }

    std::string getLocationType() const override {
        return "Exit";
    }

    bool isAvailable() const {
        return available;
    }

    void setAvailable(bool isAvailable) {
        available = isAvailable;
    }
};