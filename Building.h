#pragma once
#include <vector>
#include <iostream>
#include "Room.h"
#include "Exit.h"
#include "Hallway.h"

// building stores rooms, exits, and hallways
class Building {
private:
    std::vector<Room> rooms;
    std::vector<Exit> exits;
    std::vector<Hallway> hallways;
public:
    // adds a room
    void addRoom(const Room& room) {
        rooms.push_back(room);
    }
    // adds an emergency exit
    void addExit(const Exit& exit) {
        exits.push_back(exit);
    }
    // adds a hallway
    void addHallway(const Hallway& hallway) {
        hallways.push_back(hallway);
    }
    // returns all rooms
    const std::vector<Room>& getRooms() const {
        return rooms;
    }
    // returns all emergency exits
    const std::vector<Exit>& getExits() const {
        return exits;
    }
    // returns all hallways
    const std::vector<Hallway>& getHallways() const {
        return hallways;
    }
    // displays the types of all locations using runtime polymorphism
    void displayLocations() const {
        for (const Room& room : rooms) {
            const Location& location = room;
            std::cout << location.getName() << ": "
                      << location.getLocationType() << std::endl;
        }

        for (const Exit& exit : exits) {
            const Location& location = exit;
            std::cout << location.getName() << ": "
                      << location.getLocationType() << std::endl;
        }
    }
};
