#include "Archivos.h"

void Archivos::guardarEstaciones(vector<Station*>& stations)
{
    QDir dir("data");
    if (!dir.exists())
        dir.mkpath("data");

    ofstream file("data/estaciones.txt", ios::trunc);
    if (!file.is_open()) {
        return;
    }

    for (auto* s : stations) {
        file << s->getId() << ";"
             << s->getName() << ";"
             << s->getX() << ";"
             << s->getY() << ";"
             << s->getType() << endl;
    }

    file.close();
}

void Archivos::guardarRutas(vector<Route*>& routes)
{
    QDir dir("data");
    if (!dir.exists())
        dir.mkpath(".");

    ofstream file("data/rutas.txt");
    if (!file.is_open()) {
        return;
    }

    for (auto* r : routes)
    {
        file << r->getStart()->getId() << ";"
             << r->getEnd()->getId() << ";"
             << r->getCost() << ";"
             << (r->isClosed() ? "true" : "false") << endl;
    }

    file.close();
}

vector<Station*> Archivos::cargarEstaciones()
{
	vector<Station*> stations;
	ifstream file("data/estaciones.txt");
	if (!file.is_open()) {
		return stations;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string idStr, name, xStr, yStr, type;
		getline(ss, idStr, ';');
		getline(ss, name, ';');
		getline(ss, xStr, ';');
		getline(ss, yStr, ';');
		getline(ss, type, ';');

		int id = stoi(idStr);
		double x = stod(xStr);
		double y = stod(yStr);

		Station* s = new Station(id, name, x, y, type);
		stations.push_back(s);
	}

	file.close();
	return stations;
}

vector<Route*> Archivos::cargarRutas(vector<Station*>& stations)
{
	vector<Route*> routes;
	ifstream file("data/rutas.txt");
	if (!file.is_open()) {
		return routes;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string startIdStr, endIdStr, costStr, closedStr;
		getline(ss, startIdStr, ';');
		getline(ss, endIdStr, ';');
		getline(ss, costStr, ';');
		getline(ss, closedStr, ';');

		int startId = stoi(startIdStr);
		int endId = stoi(endIdStr);
		double cost = stod(costStr);
		bool closed = (closedStr == "1");

		Station* start = nullptr;
		Station* end = nullptr;

		for (auto* s : stations) {
			if (s->getId() == startId) start = s;
			if (s->getId() == endId) end = s;
		}

		if (start && end) {
			Route* r = new Route(start, end, cost, closed);
			routes.push_back(r);
		}
	}

	file.close();
	return routes;
}

void Archivos::guardarUsuario(User* user)
{
	ofstream file("data/users.txt", ios::app);

	QDir dir("data");
	if (!dir.exists())
		dir.mkpath(".");

	if (!file.is_open()) {
		return;
	}

	file << user->getUsername() << ";" << user->getSelectedCharacter() << endl;
	file.close();
}

vector<User*> Archivos::cargarUsuarios()
{
	vector<User*> users;

	QDir dir("data");
	if (!dir.exists())
		dir.mkpath(".");

	QFile fileCheck("data/users.txt");
	if (!fileCheck.exists()) {
		ofstream createFile("data/users.txt");
		createFile.close();
	}

	ifstream file("data/users.txt");
	if (!file.is_open()) {
		return users;
	}

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string username, character;
		getline(ss, username, ';');
		getline(ss, character, ';');

		if (!username.empty() && !character.empty()) {
			users.push_back(new User(username, character));
		}
	}

	file.close();
	return users;
}