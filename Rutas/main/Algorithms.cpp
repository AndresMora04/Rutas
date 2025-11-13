#include "Algorithms.h"

Algorithms::Algorithms() {}

void Algorithms::BFS(Graph* graph, Station* start) {
	if (start == nullptr) return;

	map<int, bool> visited;
	queue<Station*> q;

	q.push(start);
	visited[start->getId()] = true;

	while (!q.empty()) {
		Station* current = q.front();
		q.pop();
		cout << current->getName() << " ";

		vector<Route*> routes = graph->getRoutes();
		for (int i = 0; i < routes.size(); i++) {
			if (routes[i]->getStart() == current && !routes[i]->isClosed()) {
				Station* neighbor = routes[i]->getEnd();
				if (!visited[neighbor->getId()]) {
					visited[neighbor->getId()] = true;
					q.push(neighbor);
				}
			}
		}
	}

	cout << endl;
}

void Algorithms::DFS(Graph* graph, Station* start) {
	if (start == nullptr) return;

	map<int, bool> visited;
	stack<Station*> s;

	s.push(start);
	visited[start->getId()] = true;

	while (!s.empty()) {
		Station* current = s.top();
		s.pop();
		cout << current->getName() << " ";

		vector<Route*> routes = graph->getRoutes();
		for (int i = 0; i < routes.size(); i++) {
			if (routes[i]->getStart() == current && !routes[i]->isClosed()) {
				Station* neighbor = routes[i]->getEnd();
				if (!visited[neighbor->getId()]) {
					visited[neighbor->getId()] = true;
					s.push(neighbor);
				}
			}
		}
	}

	cout << endl;
}

void Algorithms::Dijkstra(Graph* graph, Station* start, Station* end) {
	if (start == nullptr || end == nullptr) return;

	vector<Station*> stations = graph->getStations();
	vector<Route*> routes = graph->getRoutes();

	map<int, double> distance;
	map<int, bool> visited;
	map<int, int> previous;

	for (int i = 0; i < stations.size(); i++) {
		distance[stations[i]->getId()] = INT_MAX;
		visited[stations[i]->getId()] = false;
		previous[stations[i]->getId()] = -1;
	}

	distance[start->getId()] = 0;

	for (int i = 0; i < stations.size() - 1; i++) {
		int minId = -1;
		double minDist = INT_MAX;

		for (int j = 0; j < stations.size(); j++) {
			int id = stations[j]->getId();
			if (!visited[id] && distance[id] < minDist) {
				minDist = distance[id];
				minId = id;
			}
		}

		if (minId == -1) break;
		visited[minId] = true;

		Station* current = graph->findStationById(minId);

		for (int j = 0; j < routes.size(); j++) {
			Route* r = routes[j];
			if (r->getStart() == current && !r->isClosed()) {
				Station* neighbor = r->getEnd();
				double newDist = distance[current->getId()] + r->getCost();

				if (newDist < distance[neighbor->getId()]) {
					distance[neighbor->getId()] = newDist;
					previous[neighbor->getId()] = current->getId();
				}
			}
		}
	}

	int currentId = end->getId();
	vector<int> path;

	while (currentId != -1) {
		path.insert(path.begin(), currentId);
		currentId = previous[currentId];
	}

	for (int i = 0; i < path.size(); i++) {
		Station* s = graph->findStationById(path[i]);
		if (s != nullptr) cout << s->getName();
		if (i < path.size() - 1) cout << " -> ";
	}

}
