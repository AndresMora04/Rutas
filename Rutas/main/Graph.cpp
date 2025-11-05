#include "Graph.h"

Graph::Graph() {}

void Graph::addStation(Station* station) {
    stations.push_back(station);
}

void Graph::addRoute(Route* route) {
    routes.push_back(route);
}

vector<Station*> Graph::getStations() {
    return stations;
}

vector<Route*> Graph::getRoutes() {
    return routes;
}

Station* Graph::findStationById(int id) {
    for (int i = 0; i < stations.size(); i++) {
        if (stations[i]->getId() == id) {
            return stations[i];
        }
    }
    return nullptr;
}

void Graph::toString() {
    cout << "Graph Info:" << endl;
    for (int i = 0; i < routes.size(); i++) {
        cout << routes[i]->getStart()->getName()
             << " -> " << routes[i]->getEnd()->getName()
             << " | Cost: " << routes[i]->getCost()
             << " | Closed: " << (routes[i]->isClosed() ? "Yes" : "No")
             << endl;
    }
}