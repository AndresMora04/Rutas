#pragma once
#include <qabstractitemmodel.h>
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <map>
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
	void toString();
	vector<Station*> getStations();
	vector<Route*> getRoutes();
	vector<Station*> bfsAvoidingBlocked(Station* start, Station* end);
	vector<Station*> dfsAvoidingBlocked(Station* start, Station* end);
	vector<Route*> kruskalAvoidingBlocked();
	vector<Route*> primAvoidingBlocked();
	vector<Station*> dijkstraAvoidingBlocked(Station* start, Station* end);
	vector<Station*> floydWarshallAvoidingBlocked(Station* start, Station* end);
	Station* findStationById(int id);
	Route* getRouteBetween(Station* start, Station* end);
};

