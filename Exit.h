#include <iostream>
using namespace std;
#include <vector>
#pragma once
class exits {
public:
    int exitid;
    bool available;
    exits(int number) {
        exitid = number;
        available = true;
    }
};