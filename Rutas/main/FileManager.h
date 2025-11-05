#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Station.h"
#include "Route.h"
#include "Graph.h"
using namespace std;

class FileManager
{
public:
    FileManager();

    vector<Station*> loadStations(string filePath);
    vector<Route*> loadRoutes(string filePath, Graph* graph);
    void saveReport(string filePath, string content);
};

