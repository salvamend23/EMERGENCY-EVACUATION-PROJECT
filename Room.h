#pragma once
#include "Location.h"
#include <vector>

class Room : public Location {
private:
    std::vector<int> connections;

public:
    Room(int number, const std::string& roomName)
        : Location(number, roomName) {
    }

    // adds a hallway connected to this room
    void addConnection(int hallwayId) {
        connections.push_back(hallwayId);
    }

    // returns the hallways connected to this room
    const std::vector<int>& getConnections() const {
        return connections;
    }

    // implements the pure virtual function from Location
    std::string getLocationType() const override {
        return "Room";
    }
};
