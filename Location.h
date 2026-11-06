#pragma once
#include <string>

//base class representing a location inside the building
//rooms and emergency exits will inherit from this class
class Location {
private:
	int id; //identifier to find a location/room
	std::string name;

public:
	Location(int locationId, const std::string& locationName)
		: id(locationId), name(locationName) {
	}

	//virtual destructor, allowing derived objects
	// = default tell the compiler to generate the standard destructor
	virtual ~Location() = default;

	int getId() const {
		return id;
	}

	std::string getName() const {
		return name;
	}

	//each derived class must implement this class 
	// = 0 makes Location an abstract class
	virtual std::string getLocationType() const = 0;
};
