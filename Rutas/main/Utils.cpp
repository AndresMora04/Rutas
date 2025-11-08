#include "Utils.h"

Station* Utils::crearEstacion(int id, string& name, QPointF pos, string type)
{
	Station* s = new Station(id, name, pos.x(), pos.y(), type);
	return s;
}

Route* Utils::crearRuta(Station* start, Station* end)
{
	double dx = end->getX() - start->getX();
	double dy = end->getY() - start->getY();
	double cost = sqrt(dx * dx + dy * dy);

	Route* r = new Route(start, end, cost, false);
	return r;
}

double Utils::calcularDistancia(Station* a, Station* b)
{
	double dx = b->getX() - a->getX();
	double dy = b->getY() - a->getY();
	return sqrt(dx * dx + dy * dy);
}

int Utils::getNextStationId() {
	ifstream file("data/estaciones.txt");
	int lastId = 0;
	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;
		stringstream ss(line);
		string idStr;
		getline(ss, idStr, ';');
		try {
			lastId = stoi(idStr);
		}
		catch (...) {
			continue;
		}
	}
	file.close();
	return lastId + 1;
}