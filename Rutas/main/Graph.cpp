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

vector<Station*> Graph::bfs(Station* start) {
    vector<Station*> visited;
    if (!start) return visited;

    map<Station*, bool> visitedMap;
    queue<Station*> q;

    q.push(start);
    visitedMap[start] = true;

    while (!q.empty()) {
        Station* current = q.front();
        q.pop();
        visited.push_back(current);

        for (auto* r : routes) {
            if (r->getStart() == current && !visitedMap[r->getEnd()]) {
                q.push(r->getEnd());
                visitedMap[r->getEnd()] = true;
            }
            else if (r->getEnd() == current && !visitedMap[r->getStart()]) {
                q.push(r->getStart());
                visitedMap[r->getStart()] = true;
            }
        }
    }
    return visited;
}

vector<Station*> Graph::dfs(Station* start) {
    vector<Station*> visited;
    if (!start) return visited;

    map<Station*, bool> visitedMap;
    stack<Station*> s;
    s.push(start);

    while (!s.empty()) {
        Station* current = s.top();
        s.pop();

        if (visitedMap[current]) continue;
        visitedMap[current] = true;
        visited.push_back(current);

        for (auto* r : routes) {
            if (r->getStart() == current && !visitedMap[r->getEnd()])
                s.push(r->getEnd());
            else if (r->getEnd() == current && !visitedMap[r->getStart()])
                s.push(r->getStart());
        }
    }
    return visited;
}

vector<Station*> Graph::dijkstra(Station* start, Station* end) {
    map<Station*, double> dist;
    map<Station*, Station*> prev;
    vector<Station*> unvisited = stations;
    vector<Station*> path;

    for (auto* s : stations)
        dist[s] = numeric_limits<double>::infinity();
    dist[start] = 0;

    while (!unvisited.empty()) {
        Station* u = nullptr;
        double minDist = numeric_limits<double>::infinity();

        for (auto* s : unvisited)
            if (dist[s] < minDist) { u = s; minDist = dist[s]; }

        if (!u) break;
        if (u == end) break;

        unvisited.erase(remove(unvisited.begin(), unvisited.end(), u), unvisited.end());

        for (auto* r : routes) {
            if (r->isClosed()) continue;

            Station* neighbor = nullptr;
            if (r->getStart() == u) neighbor = r->getEnd();
            else if (r->getEnd() == u) neighbor = r->getStart();
            else continue;

            double alt = dist[u] + r->getCost();
            if (alt < dist[neighbor]) {
                dist[neighbor] = alt;
                prev[neighbor] = u;
            }
        }
    }

    for (Station* at = end; at != nullptr; at = prev[at])
        path.insert(path.begin(), at);

    return path;
}