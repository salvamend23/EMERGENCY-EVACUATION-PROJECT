#pragma once
#include "Location.h"

//Room represents a room inside the building
// Room inherits the ID and name from Location
class Room : public Location { 
public:
	Room(int roomId, const std::string& roomName)
		: Location(roomId, roomName) {
	}

	//identify this location as a room
	std::string getLocationType() const override {
		return "Room";
	}
};