#pragma once
#include <iostream>
#include <string>
#include "Station.h"
#include "Route.h"
using namespace std;

class Event
{
private:
    string type;
    string description;
    Station* location;
    bool active;
public:
    Event();
    Event(string type, string description, Station* location);

    string getType();
    string getDescription();
    Station* getLocation();
    bool isActive();

    void resolve();
    void toString();
};

