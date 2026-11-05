#include <iostream>
using namespace std;
#include <vector>
#pragma once


class room {
public:
    int id;
    string name;
    vector<int> connections;

    room(int number, string roomname) {
        id = number;
        name = roomname;
    }
    void addconection(int hallid) {
        connections.push_back(hallid);

    }

};

class hallways {
public:
    int hallid;
    vector <int> hallconnections;
    hallways(int number) {
        hallid = number;
    }
    void addhallConnection(int location) {
        hallconnections.push_back(location);
    }
};

class exits {
public:
    int exitid;
    bool available;
    exits(int number) {
        exitid = number;
        available = true;
    }
};