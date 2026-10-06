#ifndef LOCATION_H
#define LOCATION_H

#include <string>
using namespace std;

struct Location
{
    int id;
    string name;

    Location();
    Location(int id, string name);
};

#endif