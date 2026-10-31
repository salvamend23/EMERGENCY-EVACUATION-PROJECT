#pragma once
#include "Location.h"

// represents an emergency exit in the building
class Exit : public Location {
private:
    bool available;
public:
    Exit(int exitId, const std::string& exitName)
        : Location(exitId, exitName), available(true) {
    }
    // checks whether the emergency exit is available
    bool isAvailable() const {
        return available;
    }
    // changes the availability of the exit
    void setAvailability(bool status) {
        available = status;
    }
    // implements the pure virtual function from Location
    std::string getLocationType() const override {
        return "Emergency Exit";
    }
};
