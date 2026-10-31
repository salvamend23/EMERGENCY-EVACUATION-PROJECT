#pragma once
#include <vector>

// represents a hallway connecting locations in the building
class Hallway {
private:
    int hallId;
    std::vector<int> hallConnections;
public:
    Hallway(int number) : hallId(number) {
    }
    // returns the hallway ID
    int getId() const {
        return hallId;
    }
    // adds a location connected to this hallway
    void addHallConnection(int locationId) {
        hallConnections.push_back(locationId);
    }
    // returns all locations connected to this hallway
    const std::vector<int>& getHallConnections() const {
        return hallConnections;
    }
};
