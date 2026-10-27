# EMERGENCY-EVACUATION-PROJECT
Pathfinder to an emergency

FireScape - Emergency Evacuation Simulator

Project Description
- FireScape is a C++ application designed to help users identify evacuation routes in a simplified building. The project models rooms, emergency exits, and hallways and is intended to support route finding, blocked-hallway scenarios, and exit comparisons.

Current Implementation
The initial class structure includes:
- Location.h: Abstract base class for building locations.
- Room.h: Represents a room.
- Exit.h: Represents an emergency exit and its availability.
- Hallway.h: Represents a connection between two locations.
- Building.h: Stores rooms, exits, and hallways.
- main.cpp: Console driver for testing the initial classes.
Route planning, file persistence, and the Qt graphical interface are planned features unless otherwise implemented.

Development Environment
- Operating System: Windows 10/11
- C++ Standard: C++20

Repository Structure
- The repository contains the initial class headers, the console driver, and this README.

Team
- Julia Pires
- Salvador Mendoza
- Team Name: FireScape


HALL1 = 101
hal2 = 102
hall3 = 103
hall4 = 104

exit1 = 201
exit2 = 202

rooms: {1,2,3,4,5,6,7,8,9,10,11,12}

vector<room> rooms; contains all the information of the objects so we can take all the data in order

vector<int> connections; THIS VECTOR CONTAINS WICH ROOMS ARE CONNECTED TO EACH HALLS

Hallx........ This vectors specify into what rooms each hall is conencted

HallxConnections.... This type of vectors check wich halls a hall is connected to


hallways OBJECT:::::
hallconnections vector will be for each jall object created, this will provide a vector for each individual hall object were we will store the roomIDs 
addhallconnections:::::: this wilkl allow us to put the room ids inside each hall object, this means we will have the information on were each hall has connections to what room.


EXITCLASS::::
the bool will intervene to determine wether the exit is open or closed
