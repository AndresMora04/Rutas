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

Route* Graph::getRouteBetween(Station* start, Station* end)
{
	if (!start || !end) return nullptr;

	for (auto* r : routes) {
		if (!r || !r->getStart() || !r->getEnd())
			continue;

		int startId = start->getId();
		int endId = end->getId();

		if ((r->getStart()->getId() == startId && r->getEnd()->getId() == endId) ||
			(r->getStart()->getId() == endId && r->getEnd()->getId() == startId)) {
			return r;
		}
	}
	return nullptr;
}

void Graph::toString() {
	cout << "Informacion grafica:" << endl;
	for (int i = 0; i < routes.size(); i++) {
		cout << routes[i]->getStart()->getName()
			<< " -> " << routes[i]->getEnd()->getName()
			<< " | Costo: " << routes[i]->getCost()
			<< " | Cerrado: " << (routes[i]->isClosed() ? "Si" : "No")
			<< endl;
	}
    cout << endl;
}

vector<Station*> Graph::bfsAvoidingBlocked(Station* start, Station* end)
{
	vector<Station*> path;
	if (!start || !end) return path;

	if (start == end) {
		path.push_back(start);
		return path;
	}

	map<Station*, bool> visited;
	map<Station*, Station*> prev;
	queue<Station*> q;

	q.push(start);
	visited[start] = true;

	while (!q.empty()) {
		Station* current = q.front();
		q.pop();

		if (current == end)
			break;

		for (auto* r : routes) {
			if (r->isClosed()) continue;
			Station* neighbor = nullptr;

			if (r->getStart() == current) neighbor = r->getEnd();
			else if (r->getEnd() == current) neighbor = r->getStart();

			if (!neighbor || neighbor->isBlocked()) continue;

			if (!visited[neighbor]) {
				visited[neighbor] = true;
				prev[neighbor] = current;
				q.push(neighbor);
			}
		}
	}

	Station* curr = end;
	while (curr && prev.count(curr)) {
		path.insert(path.begin(), curr);
		curr = prev[curr];
	}

	if (curr == start)
		path.insert(path.begin(), start);

	return path;
}

vector<Station*> Graph::dfsAvoidingBlocked(Station* start, Station* end)
{
	vector<Station*> path;
	if (!start || !end) return path;

	map<Station*, bool> visited;
	map<Station*, Station*> prev;
	stack<Station*> s;

	s.push(start);

	while (!s.empty()) {
		Station* current = s.top();
		s.pop();

		if (visited[current] || current->isBlocked()) continue;
		visited[current] = true;

		if (current == end)
			break;

		for (auto* r : routes) {
			if (r->isClosed()) continue;
			Station* neighbor = nullptr;

			if (r->getStart() == current) neighbor = r->getEnd();
			else if (r->getEnd() == current) neighbor = r->getStart();

			if (!neighbor || neighbor->isBlocked()) continue;

			if (!visited[neighbor]) {
				prev[neighbor] = current;
				s.push(neighbor);
			}
		}
	}

	Station* curr = end;
	while (curr && prev.count(curr)) {
		path.insert(path.begin(), curr);
		curr = prev[curr];
	}

	if (curr == start)
		path.insert(path.begin(), start);

	return path;
}

vector<Station*> Graph::dijkstraAvoidingBlocked(Station* start, Station* end)
{
	vector<Station*> path;
	if (!start || !end) return path;

	map<Station*, double> dist;
	map<Station*, Station*> prev;
	vector<Station*> unvisited = stations;

	for (auto* s : stations)
		dist[s] = numeric_limits<double>::infinity();

	dist[start] = 0;

	while (!unvisited.empty()) {
		Station* u = nullptr;
		double minDist = numeric_limits<double>::infinity();
		for (auto* s : unvisited)
			if (!s->isBlocked() && dist[s] < minDist) {
				u = s;
				minDist = dist[s];
			}

		if (!u || u == end)
			break;

		unvisited.erase(remove(unvisited.begin(), unvisited.end(), u), unvisited.end());

		for (auto* r : routes) {
			if (r->isClosed()) continue;
			Station* neighbor = nullptr;
			if (r->getStart() == u) neighbor = r->getEnd();
			else if (r->getEnd() == u) neighbor = r->getStart();

			if (!neighbor || neighbor->isBlocked()) continue;

			double alt = dist[u] + r->getCost();
			if (alt < dist[neighbor]) {
				dist[neighbor] = alt;
				prev[neighbor] = u;
			}
		}
	}

	Station* curr = end;
	while (curr && prev.count(curr)) {
		path.insert(path.begin(), curr);
		curr = prev[curr];
	}

	if (curr == start)
		path.insert(path.begin(), start);

	return path;
}

vector<Station*> Graph::floydWarshallAvoidingBlocked(Station* start, Station* end)
{
	vector<Station*> path;
	if (!start || !end) return path;
	int n = stations.size();
	map<Station*, int> indexMap;
	for (int i = 0; i < n; ++i)
		indexMap[stations[i]] = i;
	vector<vector<double>> dist(n, vector<double>(n, numeric_limits<double>::infinity()));
	vector<vector<Station*>> next(n, vector<Station*>(n, nullptr));
	for (int i = 0; i < n; ++i) {
		Station* u = stations[i];
		if (u->isBlocked()) continue;
		dist[i][i] = 0;
		for (auto* r : routes) {
			if (r->isClosed()) continue;
			Station* v = nullptr;
			if (r->getStart() == u) v = r->getEnd();
			else if (r->getEnd() == u) v = r->getStart();
			if (!v || v->isBlocked()) continue;
			int j = indexMap[v];
			dist[i][j] = r->getCost();
			next[i][j] = v;
		}
	}
	for (int k = 0; k < n; ++k) {
		Station* mid = stations[k];
		if (mid->isBlocked()) continue;
		for (int i = 0; i < n; ++i) {
			Station* u = stations[i];
			if (u->isBlocked()) continue;
			for (int j = 0; j < n; ++j) {
				Station* v = stations[j];
				if (v->isBlocked()) continue;
				if (dist[i][k] + dist[k][j] < dist[i][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
					next[i][j] = next[i][k];
				}
			}
		}
	}
	int startIdx = indexMap[start];
	int endIdx = indexMap[end];
	if (next[startIdx][endIdx] == nullptr)
		return path;
	Station* curr = start;
	while (curr != end) {
		path.push_back(curr);
		curr = next[indexMap[curr]][endIdx];
	}
	path.push_back(end);
	return path;
}

vector<Route*> Graph::kruskalAvoidingBlocked()
{
	vector<Route*> result;
	vector<Route*> edges;

	for (auto* r : routes)
		if (!r->isClosed() && !r->getStart()->isBlocked() && !r->getEnd()->isBlocked())
			edges.push_back(r);

	sort(edges.begin(), edges.end(), [](Route* a, Route* b) {
		return a->getCost() < b->getCost();
		});

	map<Station*, Station*> parent;
	function<Station* (Station*)> find = [&](Station* s) {
		return (parent[s] == s) ? s : parent[s] = find(parent[s]);
		};

	for (auto* s : stations)
		if (!s->isBlocked())
			parent[s] = s;

	for (auto* e : edges) {
		Station* u = find(e->getStart());
		Station* v = find(e->getEnd());
		if (u != v) {
			result.push_back(e);
			parent[u] = v;
		}
	}

	return result;
}

vector<Route*> Graph::primAvoidingBlocked()
{
	vector<Route*> mst;
	if (stations.empty()) return mst;

	vector<Station*> available;
	for (auto* s : stations)
		if (!s->isBlocked())
			available.push_back(s);

	if (available.empty()) return mst;

	map<Station*, bool> inMST;
	Station* start = available.front();
	inMST[start] = true;

	while (mst.size() < available.size() - 1) {
		double minCost = numeric_limits<double>::infinity();
		Route* best = nullptr;

		for (auto* r : routes) {
			if (r->isClosed()) continue;
			Station* u = r->getStart();
			Station* v = r->getEnd();

			if (u->isBlocked() || v->isBlocked()) continue;

			if (inMST[u] && !inMST[v] && r->getCost() < minCost) {
				minCost = r->getCost(); best = r;
			}
			else if (inMST[v] && !inMST[u] && r->getCost() < minCost) {
				minCost = r->getCost(); best = r;
			}
		}

		if (!best) break;

		mst.push_back(best);
		inMST[best->getStart()] = true;
		inMST[best->getEnd()] = true;
	}

	return mst;
}