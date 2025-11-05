#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Route.h"
#include "Event.h"
using namespace std;

class Randomizer
{
public:
 Randomizer();

    Event* generateRandomEvent(vector<Route*>& routes);
    void resetRandom();
};