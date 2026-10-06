#include "location.h"

Location::Location()
{
    id = 0;
    name = "";
}

Location::Location(int id, string name)
{
    this->id = id;
    this->name = name;
}