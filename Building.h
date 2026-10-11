#pragma once
#include <vector>
#include "Room.h"
#include "Exit.h"
#include "Hallway.h"

// Represents the building and stores its locations and hallways.
class Building {
private:
    std::vector<Room> rooms;
    std::vector<Exit> exits;
    std::vector<Hallway> hallways;

public:
    // Adds a room to the building.
    void addRoom(const Room& room) {
        rooms.push_back(room);
    }

    // Adds an emergency exit to the building.
    void addExit(const Exit& exit) {
        exits.push_back(exit);
    }

    // Adds a hallway to the building.
    void addHallway(const Hallway& hallway) {
        hallways.push_back(hallway);
    }

    // Returns all rooms.
    const std::vector<Room>& getRooms() const {
        return rooms;
    }

    // Returns all emergency exits.
    const std::vector<Exit>& getExits() const {
        return exits;
    }

    // Returns all hallways.
    const std::vector<Hallway>& getHallways() const {
        return hallways;
    }
};