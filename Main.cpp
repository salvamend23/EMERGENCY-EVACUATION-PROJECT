// PROJECT DEMO PRACTICE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <limits>
#include <algorithm>
#include <stdexcept>

#include "Room.h"
#include "Hallway.h"
#include "Exit.h"
#include "Pathfinder.h"

using namespace std;


int main()

{
    vector<Room> rooms;
    Hallway hall1(101);
    Hallway hall2(102);
    Hallway hall3(103);
    Hallway hall4(104);
    Exit exit1(201, "Exit 1");
    Exit exit2(202, "Exit 2");


    Room a(1, "Conference Room");
    Room b(2, "kitchen");

    rooms.push_back(a);
    rooms.push_back(b);
    rooms.push_back(Room(3, "Living Room"));
    rooms.push_back(Room(4, "Bedroom"));
    rooms.push_back(Room(5, "Bathroom"));
    rooms.push_back(Room(6, "Dining Room"));
    rooms.push_back(Room(7, "Game Room"));
    rooms.push_back(Room(8, "Study Room"));
    rooms.push_back(Room(9, "Lanai"));
    rooms.push_back(Room(10, "Teachers Room"));
    rooms.push_back(Room(11, "Club Room"));
    rooms.push_back(Room(12, "Hobby Lobby"));


    // DISPLAY ROOMS
    for (Room& currentRoom : rooms) {
        cout << "Room ID: " << currentRoom.getId()
        << " Name: " << currentRoom.getName() << endl;
    }

    // CONNECT THE ROOMS TO HALLWAYS
    for (Room& currentRoom : rooms) {

        if (currentRoom.getId() >= 1 && currentRoom.getId() <= 4) {
            currentRoom.addConnection(101);
        }

        if (currentRoom.getId() >= 5 && currentRoom.getId() <= 6) {
            currentRoom.addConnection(102);
        }

        if (currentRoom.getId() >= 7 && currentRoom.getId() <= 10) {
            currentRoom.addConnection(103);
        }

        if (currentRoom.getId() >= 11 && currentRoom.getId() <= 12) {
            currentRoom.addConnection(104);
        }
    }
    
    //START THE TEST TO BE SURE THE ROOM CONNECTS TO ITS RESPECTIVE HALL
    cout << "\n";
    for (Room& currentRoom : rooms) {
        cout << "Room " << currentRoom.getId()
        << " connects to hall ";

        for (int id : currentRoom.getConnections()) {
            cout << id << " ";
        }

        cout << endl;
    }
    // END OF TEST

    cout << "" << endl;
    // FILLING HALLWAY OBJECTS OBJECTS
    for (Room& currentRoom : rooms) {

        if (currentRoom.getId() >= 1 && currentRoom.getId() <= 4) {
            hall1.addHallConnection(currentRoom.getId());
        }

        if (currentRoom.getId() >= 5 && currentRoom.getId() <= 6) {
            hall2.addHallConnection(currentRoom.getId());
        }

        if (currentRoom.getId() >= 7 && currentRoom.getId() <= 10) {
            hall3.addHallConnection(currentRoom.getId());
        }

        if (currentRoom.getId() >= 11 && currentRoom.getId() <= 12) {
            hall4.addHallConnection(currentRoom.getId());
        }
    }  

    // TEST HALLWAY CONNECTIONS
    cout << "\n";

    cout << "Rooms connected to hall 1: ";
    for (int id : hall1.getHallConnections()) {
        cout << id << " ";
    }

    cout << "\nRooms connected to hall 2: ";
    for (int id : hall2.getHallConnections()) {
        cout << id << " ";
    }

    cout << "\nRooms connected to hall 3: ";
    for (int id : hall3.getHallConnections()) {
        cout << id << " ";
    }

    cout << "\nRooms connected to hall 4: ";
    for (int id : hall4.getHallConnections()) {
        cout << id << " ";
    }

    cout << "\n";
   
    // TEST EXIT INITIALIZATION
    vector<Exit> exitLocations;
    exitLocations.push_back(exit1);
    exitLocations.push_back(exit2);

    for (const Exit& currentExit : exitLocations) {
        cout << "Exit Number: " << currentExit.getId() << endl;
        cout << "Availability: " << currentExit.isAvailable() << endl;
    }
    //END OF TEST




    // CONNECTIONS OF HALLS BETWEEN HALLS
    hall1.addHallConnection(102);
    hall1.addHallConnection(104);
    hall1.addHallConnection(201);


    hall2.addHallConnection(101);
    hall2.addHallConnection(103);


    hall3.addHallConnection(102);
    hall3.addHallConnection(104);
    hall3.addHallConnection(202);


    hall4.addHallConnection(103);
    hall4.addHallConnection(101);

    // END OF HALL CONECTIONS

    //TEST THE ENTIRE HALL CONNECTION
    cout << "\nConnections in hall1: ";
    for (int id : hall1.getHallConnections()) {
        cout << id << " ";
    }
    cout << "\nConnections in hall2: ";
    for (int id : hall2.getHallConnections()) {
        cout << id << " ";
    }
    cout << "\nConnections in hall3: ";
    for (int id : hall3.getHallConnections()) {
        cout << id << " ";
    }
    cout << "\nConnections in hall4: ";
    for (int id : hall4.getHallConnections()) {
        cout << id << " ";
    }


    // MERGE ALL HALLWAYS INTO ONE VECTOR
    vector <Hallway> hallways;
    hallways.push_back(hall1);
    hallways.push_back(hall2);
    hallways.push_back(hall3);
    hallways.push_back(hall4);

    cout << "\n\n";
    for (const Hallway& currentHallway : hallways) {
        cout << "Hallway ID: " << currentHallway.getId() << endl;
    }
    cout << "\n\n";

// TESTING HALLWAY BLOCKING AND EXCEPTION HANDLING
cout << "\nTESTING HALLWAY BLOCKING:\n";

try {
    hall1.block();
    cout << "Hallway 101 blocked successfully.\n";

    hall1.block();
    cout << "Hallway 101 blocked again.\n";
}
catch (const logic_error& e) {
    cout << "Error: " << e.what() << endl;
}

cout << "Hallway 101 blocked status: "
     << hall1.isBlocked() << endl;

cout << "Program continues running.\n";

    //// initialization of pathfinder

    Pathfinder pathfinder;
    pathfinder.addconnections(101, 102, 20.0);
    pathfinder.addconnections(101, 104, 25.0);
    pathfinder.addconnections(102, 103, 20.0);
    pathfinder.addconnections(103, 104, 25.0);


    for (Connection c : pathfinder.connections) {
        cout << "from " << c.from << "to --> " << c.to << ", Distance: " << c.distance << "METERS" << endl;

    }
    // ADD CONNECTIONS OF ROOMS TO HALLWAYS BY DISTANCE
    for (const Room& r : rooms) {
        for (int hallid : r.getConnections()) {
            pathfinder.addconnections(r.getId(), hallid, 10.0);
        }
    }
    // CONENCT EXIT TO THE HALLWAY BY DISTANCE
    pathfinder.addconnections(101, 201, 3.0);
    pathfinder.addconnections(103, 202, 3.0);

    cout << "\nTotal connections: " << pathfinder.connections.size() << endl;


    // NEIGHBOR TEST
    cout << "\n\nNeighbors of hall 101: " << endl;
    pathfinder.showneighbors(101);
    cout << "\nNeighbors of hall 103: " << endl;
    pathfinder.showneighbors(103);

    cout << "\nNeighbors of room 8: " << endl;
    pathfinder.showneighbors(8);



    //START OF DJIKSTRA ALGORITHM
    cout << "\nTESTING DIJKSTRA:\n";


    int blockedHallwayId = hall1.isBlocked() ? hall1.getId() : -1;
    path_results results = pathfinder.calculate_distances(5, blockedHallwayId, 202);// TESTING THE EMERGENCY SIMULATION FUNCTION

    cout << "\nDEBUG found: " << results.found << endl;
    cout << "DEBUG exit: " << results.exitid << endl;
    cout << "DEBUG route size: " << results.route.size() << endl;


    if (results.found) {
        cout << "\nNearest Exit id: " << results.exitid;

        cout << "\nBest Evacuation route: ";
        for (int i : results.route) {
            cout << i << " ";
            if (i != results.route.back()) {
                cout << "-> ";
            }
        }
        cout << "\nTotal distance: " << results.total_distance << " METERS" << endl;




    }


    
    pathfinder.compare_routes(5, blockedHallwayId); // TESTING THE COMPARE ROUTES FUNCTION

    cout << "\n\n";
    // TESTING EXCEPTION HANDLING
    try {
        int testRoom = 15;

        if (testRoom < 1 || testRoom > 12) {
            throw invalid_argument("Invalid room ID! Room must be between 1 and 12.");
        }

        cout << "\nValid room: " << testRoom << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "\nERROR: " << e.what() << endl;
    }



}


