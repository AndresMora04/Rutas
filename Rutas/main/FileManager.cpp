#include "FileManager.h"

FileManager::FileManager() {}

vector<Station*> FileManager::loadStations(string filePath) {
    vector<Station*> stations;
    ifstream file(filePath);

    if (!file.is_open()) {
        cout << "Error opening stations file: " << filePath << endl;
        return stations;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id;
        string name;
        double x, y;

        getline(ss, name, ';');
        ss.clear();
        ss.str(line);
        ss >> id;
        getline(ss, name, ';');
        ss >> x >> y;

        Station* s = new Station(id, name, x, y);
        stations.push_back(s);
    }

    file.close();
    cout << "Loaded " << stations.size() << " stations." << endl;
    return stations;
}

vector<Route*> FileManager::loadRoutes(string filePath, Graph* graph) {
    vector<Route*> routes;
    ifstream file(filePath);

    if (!file.is_open()) {
        cout << "Error opening routes file: " << filePath << endl;
        return routes;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int startId, endId;
        double cost;
        bool closed = false;

        string token;
        getline(ss, token, ';');
        startId = stoi(token);

        getline(ss, token, ';');
        endId = stoi(token);

        getline(ss, token, ';');
        cost = stod(token);

        getline(ss, token, ';');
        closed = (token == "1");

        Station* start = graph->findStationById(startId);
        Station* end = graph->findStationById(endId);

        if (start != nullptr && end != nullptr) {
            Route* r = new Route(start, end, cost, closed);
            routes.push_back(r);
        }
    }

    file.close();
    cout << "Loaded " << routes.size() << " routes." << endl;
    return routes;
}

void FileManager::saveReport(string filePath, string content) {
    ofstream file(filePath, ios::app);
    if (!file.is_open()) {
        cout << "Error opening report file: " << filePath << endl;
        return;
    }

    file << content << endl;
    file.close();
    cout << "Report saved to " << filePath << endl;
}