#pragma once
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

	vector<Station*> getStations();
	vector<Route*> getRoutes();

	Station* findStationById(int id);
	void toString();

	vector<Station*> bfs(Station* start);
	vector<Station*> dfs(Station* start);
	vector<Station*> dijkstra(Station* start, Station* end);
};

