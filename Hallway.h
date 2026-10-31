#pragma once
#include <vector>
#include <stdexcept>

// represents a hallway connecting locations in the building
class Hallway {
private:
    int hallId;
    std::vector<int> hallConnections;
    bool blocked = false;
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
    // blocks the hallway
        void block() {
            if (blocked) {
                throw std::logic_error("This hallway is already blocked.");
            }

            blocked = true;
        }

        bool isBlocked() const {
            return blocked;
        }
};
