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