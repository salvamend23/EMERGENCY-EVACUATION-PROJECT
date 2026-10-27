#include <iostream>
using namespace std;
#include <vector>
#pragma once



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
