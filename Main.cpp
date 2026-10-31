#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "Room.h"
#include "Hallway.h"
#include "Exit.h"
#include "Building.h"

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
    rooms.push_back(Room(7, "game room"));
    rooms.push_back(Room(8, "study room"));
    rooms.push_back(Room(9, "lanai"));
    rooms.push_back(Room(10, "teachers room"));
    rooms.push_back(Room(11, "club room"));
    rooms.push_back(Room(12, "Hobby lobby"));


    for (Room& currentRoom : rooms) {
        cout << "Room ID: " << currentRoom.getId() << " Name: " << currentRoom.getName() << endl;

    }

    //CONNECT THE ROOMS TO HALLWAY
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
        cout << "Room " << currentRoom.getId() << " connects to hall ";
        for (int id : currentRoom.getConnections()) {
            cout << id << "";
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
    ////END OF SETUP HALLx

    cout << "\n" << endl;
    cout << "Rooms connected to hall 1: ";
    for (int id : hall1.getHallConnections()) {
        cout << id << " ";
    }

    cout << "\n" << endl;
    cout << "Rooms connected to hall 2: ";
    for (int id : hall2.getHallConnections()) {
        cout << id << " ";
    }

    cout << "\n" << endl;
    cout << "Rooms connected to hall 3: ";

    for (int id : hall3.getHallConnections()) {
        cout << id << " ";
    }

    cout << "\n" << endl;
    cout << "Rooms connected to hall 4: ";
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
        cout << "Name: " << currentExit.getName() << endl;
        cout << "Availability: "
         << (currentExit.isAvailable() ? "Available" : "Unavailable")
         << endl;
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

    // TEST BUILDING AND RUNTIME POLYMORPHISM
    Building building;

    for (const Room& currentRoom : rooms) {
        building.addRoom(currentRoom);
    }

    building.addExit(exit1);
    building.addExit(exit2);

    building.addHallway(hall1);
    building.addHallway(hall2);
    building.addHallway(hall3);
    building.addHallway(hall4);

    cout << "\nLocations in the building:\n";
    building.displayLocations();


}
