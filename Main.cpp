// PROJECT DEMO PRACTICE.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <limits>
#include <algorithm>

#include "Room.h"
#include "Hallway.h"
#include "Exit.h"
#include "Pathfinder.h"

using namespace std;


int main()

{
    vector<room> rooms;
    hallways hall1(101);
    hallways hall2(102);
    hallways hall3(103);
    hallways hall4(104);
    exits exit1(201);
    exits exit2(202);


    room a(1, "Conference Room");
    room b(2, "kitchen");

    rooms.push_back(a);
    rooms.push_back(b);
    rooms.push_back(room(3, "Living Room"));
    rooms.push_back(room(4, "Bedroom"));
    rooms.push_back(room(5, "Bathroom"));
    rooms.push_back(room(6, "Dining Room"));
    rooms.push_back(room(7, "game room"));
    rooms.push_back(room(8, "study room"));
    rooms.push_back(room(9, "lanai"));
    rooms.push_back(room(10, "teachers room"));
    rooms.push_back(room(11, "club room"));
    rooms.push_back(room(12, "Hobby lobby"));


    for (room& rooms : rooms) {
        cout << "room id: " << rooms.id << " name: " << rooms.name << endl;

    }

    //CONNECT THE ROOMS TO HALLWAY
    for (room& x : rooms) {
        if (x.id >= 1 && x.id <= 4) {
            x.addconection(101);
        }
        if (x.id >= 5 && x.id <= 6) {
            x.addconection(102);
        }
        if (x.id >= 7 && x.id <= 10) {
            x.addconection(103);
        }
        if (x.id >= 11 && x.id <= 12) {
            x.addconection(104);
        }
    }
    //START THE TEST TO BE SURE THE ROOM CONNECTS TO ITS RESPECTIVE HALL
    cout << "\n";
    for (room& x : rooms) {
        cout << "Room " << x.id << " connects to hall ";
        for (int id : x.connections) {
            cout << id << "";
        }
        cout << endl;
    }
    // END OF TEST




    cout << "" << endl;
    // FILLING HALLWAY OBJECTS OBJECTS
    for (room& Rooms : rooms) {

        if (Rooms.id >= 1 && Rooms.id <= 4) {
            hall1.addhallConnection(Rooms.id);
        }
        if (Rooms.id >= 5 && Rooms.id <= 6) {
            hall2.addhallConnection(Rooms.id);
        }
        if (Rooms.id >= 7 && Rooms.id <= 10) {
            hall3.addhallConnection(Rooms.id);

        }
        if (Rooms.id >= 11 && Rooms.id <= 12) {
            hall4.addhallConnection(Rooms.id);
        }
    }
    ////END OF SETUP HALLx

    cout << "\n" << endl;
    cout << "rooms id connected to hall 1: ";
    for (int i : hall1.hallconnections) {
        cout << i << " ";
    }

    cout << "\n" << endl;
    cout << "rooms id connected to hall 2: ";
    for (int i : hall2.hallconnections) {
        cout << i << " ";
    }

    cout << "\n" << endl;
    cout << "rooms id connected to hall 3: ";

    for (int i : hall3.hallconnections) {
        cout << i << " ";
    }

    cout << "\n" << endl;
    cout << "rooms id connected to hall 4: ";
    for (int i : hall4.hallconnections) {
        cout << i << " ";
    }
    cout << "\n";

    // TEST EXIT INITIALIZATION
    vector<exits> exitlocations;
    exitlocations.push_back(exit1);
    exitlocations.push_back(exit2);

    for (exits& number : exitlocations) {
        cout << "exit Number: " << number.exitid << endl;
        cout << "availability: " << number.available << endl;
    }


    //END OF TEST




    // CONNECTIONS OF HALLS BETWEEN HALLS



    hall1.addhallConnection(102);
    hall1.addhallConnection(104);
    hall1.addhallConnection(201);


    hall2.addhallConnection(101);
    hall2.addhallConnection(103);


    hall3.addhallConnection(102);
    hall3.addhallConnection(104);
    hall3.addhallConnection(202);


    hall4.addhallConnection(103);
    hall4.addhallConnection(101);

    // END OF HALL CONECTIONS

    //TEST THE ENTIRE HALL CONNECTION
    cout << "\nConnections in hall1: ";
    for (int Id : hall1.hallconnections) {
        cout << Id << " ";
    }
    cout << "\nConnections in hall2: ";
    for (int Id : hall2.hallconnections) {
        cout << Id << " ";
    }
    cout << "\nConnections in hall3: ";
    for (int Id : hall3.hallconnections) {
        cout << Id << " ";
    }
    cout << "\nConnections in hall4: ";
    for (int Id : hall4.hallconnections) {
        cout << Id << " ";
    }


    // MERGE ALL HALLWAYS INTO ONE VECTOR
    vector <hallways> hallway;
    hallway.push_back(hall1);
    hallway.push_back(hall2);
    hallway.push_back(hall3);
    hallway.push_back(hall4);

    cout << "\n\n";
    for (hallways& id : hallway) {
        cout << "Hallway id: " << id.hallid << endl;
    }
    cout << "\n\n";



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
    for (room& r : rooms) {
        for (int hallid : r.connections) {
            pathfinder.addconnections(r.id, hallid, 10.0);
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






    path_results results = pathfinder.simulate_emergency(5, 202);// TESTING THE EMERGENCY SIMULATION FUNCTION

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


    cout << "\n\n";
    pathfinder.compare_routes(5); // TESTING THE COMPARE ROUTES FUNCTION






}


