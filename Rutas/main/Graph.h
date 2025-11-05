#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Station.h"
#include "Route.h"
using namespace std;

class Graph
{
private:
    vector<Station*> stations;
    vector<Route*> routes;

public:
    Graph();

    void addStation(Station* station);
    void addRoute(Route* route);

    vector<Station*> getStations();
    vector<Route*> getRoutes();

    Station* findStationById(int id);
    void toString();
};

