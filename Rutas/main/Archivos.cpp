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

	inicializarArchivosUsuario(user->getUsername());
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

void Archivos::guardarEstacionesUsuario(const string& username, vector<Station*>& stations)
{
	string folderPath = "data/usuarios/" + username;


	string filePath = folderPath + "/estaciones.txt";
	ofstream file(filePath, ios::trunc);

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

vector<Station*> Archivos::cargarEstacionesUsuario(const string& username)
{
	vector<Station*> stations;
	string filePath = "data/usuarios/" + username + "/estaciones.txt";

	QDir dir(QString::fromStdString("data/usuarios/" + username));
	if (!dir.exists())
		dir.mkpath(".");

	ifstream file(filePath);
	if (!file.is_open()) return stations;

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string idStr, name, xStr, yStr, type;
		getline(ss, idStr, ';');
		getline(ss, name, ';');
		getline(ss, xStr, ';');
		getline(ss, yStr, ';');
		getline(ss, type, ';');

		if (idStr.empty() || xStr.empty() || yStr.empty())
			continue;

		int id = stoi(idStr);
		double x = stod(xStr);
		double y = stod(yStr);
		Station* s = new Station(id, name, x, y, type);

		bool exists = false;
		for (auto* existing : stations) {
			if (existing->getId() == id || existing->getName() == name) {
				exists = true;
				break;
			}
		}

		if (!exists) {
			Station* s = new Station(id, name, x, y, type);
			stations.push_back(s);
		}
	}
	file.close();
	return stations;
}

void Archivos::guardarRutasUsuario(const string& username, vector<Route*>& routes)
{
	string folderPath = "data/usuarios/" + username;
	QDir dir(QString::fromStdString(folderPath));
	if (!dir.exists())
		dir.mkpath(".");

	string filePath = folderPath + "/rutas.txt";
	ofstream file(filePath, ios::trunc);
	if (!file.is_open()) return;

	for (auto* r : routes) {
		file << r->getStart()->getId() << ";"
			<< r->getEnd()->getId() << ";"
			<< r->getCost() << ";"
			<< (r->isClosed() ? "true" : "false") << endl;
	}
	file.close();
}

vector<Route*> Archivos::cargarRutasUsuario(const string& username, vector<Station*>& stations)
{
	vector<Route*> routes;
	string filePath = "data/usuarios/" + username + "/rutas.txt";

	QDir dir(QString::fromStdString("data/usuarios/" + username));
	if (!dir.exists())
		dir.mkpath(".");

	ifstream file(filePath);
	if (!file.is_open()) return routes;

	string line;
	while (getline(file, line)) {
		stringstream ss(line);
		string startIdStr, endIdStr, costStr, closedStr;
		getline(ss, startIdStr, ';');
		getline(ss, endIdStr, ';');
		getline(ss, costStr, ';');
		getline(ss, closedStr, ';');

		if (startIdStr.empty() || endIdStr.empty() || costStr.empty())
			continue;

		int startId = stoi(startIdStr);
		int endId = stoi(endIdStr);
		double cost = stod(costStr);
		bool closed = (closedStr == "true");

		Station* start = nullptr;
		Station* end = nullptr;

		for (auto* s : stations) {
			if (s->getId() == startId) start = s;
			if (s->getId() == endId) end = s;
		}

		if (start && end)
			routes.push_back(new Route(start, end, cost, closed));
	}

	file.close();
	return routes;
}

vector<Report> Archivos::cargarReportesUsuario(const string& username)
{
	vector<Report> reports;
	string filePath = "data/usuarios/" + username + "/reportes.txt";
	ifstream file(filePath);
	if (!file.is_open()) return reports;

	string line;
	string currentTitle, player, character, result;
	double totalCost = 0;
	vector<string> visited;

	while (getline(file, line)) {
		if (line.rfind("Reporte:", 0) == 0) {
			if (!currentTitle.empty()) {
				Report r(currentTitle, player, character);
				for (auto& s : visited) r.addVisitedStation(s);
				r.setTotalCost(totalCost);
				r.setResult(result);
				reports.push_back(r);

				visited.clear();
				totalCost = 0;
				result = "";
			}
			currentTitle = line.substr(8);
		}
		else if (line.rfind("Usuario:", 0) == 0) {
			size_t pipe = line.find('|');
			player = line.substr(8, pipe - 9);
			character = line.substr(pipe + 13);
		}
		else if (line.rfind("Visitado:", 0) == 0) {
			string names = line.substr(9);
			stringstream ss(names);
			string name;
			while (getline(ss, name, '-')) {
				if (name != ">" && !name.empty())
					visited.push_back(name);
			}
		}
		else if (line.rfind("Costo Total:", 0) == 0) {
			totalCost = stod(line.substr(12));
		}
		else if (line.rfind("Resultado:", 0) == 0) {
			result = line.substr(8);
		}
	}

	if (!currentTitle.empty()) {
		Report r(currentTitle, player, character);
		for (auto& s : visited) r.addVisitedStation(s);
		r.setTotalCost(totalCost);
		r.setResult(result);
		reports.push_back(r);
	}

	file.close();
	return reports;
}

void Archivos::guardarRecorridosUsuario(const string& username,
	const vector<std::string>& inorder,
	const vector<std::string>& preorder,
	const vector<std::string>& postorder)
{
	string folderPath = "data/usuarios/" + username;
	QDir dir(QString::fromStdString(folderPath));
	if (!dir.exists())
		dir.mkpath(".");

	string filePath = folderPath + "/recorridos_rutas.txt";
	ofstream file(filePath, ios::trunc);
	if (!file.is_open()) {
		return;
	}

	file << "INORDER:\n";
	for (auto& n : inorder)
		file << n << " ? ";
	file << "\n\nPREORDER:\n";
	for (auto& n : preorder)
		file << n << " ? ";
	file << "\n\nPOSTORDER:\n";
	for (auto& n : postorder)
		file << n << " ? ";

	file.close();
}

void Archivos::guardarCierresUsuario(const string& username, const vector<Route*>& routes)
{
	string folderPath = "data/usuarios/" + username;
	QDir dir(QString::fromStdString(folderPath));
	if (!dir.exists())
		dir.mkpath(".");

	string filePath = folderPath + "/cierres.txt";
	ofstream file(filePath, ios::trunc);

	if (!file.is_open()) {
		return;
	}

	for (auto* r : routes) {
		if (r->isClosed()) {
			file << r->getStart()->getId() << ";" << r->getEnd()->getId() << endl;
		}
	}

	file.close();
}

vector<pair<int, int>> Archivos::cargarCierresUsuario(const string& username)
{
	vector<pair<int, int>> cierres;
	string filePath = "data/usuarios/" + username + "/cierres.txt";

	ifstream file(filePath);
	if (!file.is_open()) {
		return cierres;
	}

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		stringstream ss(line);
		string startIdStr, endIdStr;

		if (getline(ss, startIdStr, ';') && getline(ss, endIdStr)) {
			try {
				if (startIdStr.empty() || endIdStr.empty())
					continue;

				try {
					int startId = stoi(startIdStr);
					int endId = stoi(endIdStr);
					cierres.emplace_back(startId, endId);
				}
				catch (...) {
					continue;
				}
			}
			catch (const invalid_argument&) {
				continue;
			}
		}
	}

	file.close();

	return cierres;
}

void Archivos::guardarCierresUsuario(const string& username)
{
	string folderPath = "data/usuarios/" + username;
	QDir dir(QString::fromStdString(folderPath));
	if (!dir.exists())
		dir.mkpath(".");
	string filePath = folderPath + "/cierres.txt";
	ofstream file(filePath, ios::trunc);
	if (!file.is_open()) {
		return;
	}
	file.close();
}

string Archivos::obtenerRutaUsuario(const string& username)
{
	string folderPath = "data/usuarios/" + username;
	QDir dir(QString::fromStdString(folderPath));
	if (!dir.exists())
		dir.mkpath(".");
	return folderPath;
}

void Archivos::inicializarArchivosUsuario(const string& username)
{
	if (username.empty()) {
		return;
	}

	QString userFolder = QString::fromUtf8(username.c_str());

	QDir baseDir(QDir::currentPath() + "/data/usuarios/");
	QString folderPath = baseDir.absoluteFilePath(userFolder);

	QDir dir(folderPath);
	if (!dir.exists()) {
		if (dir.mkpath(".")) {
		}
		else {
			return;
		}
	}

	QStringList archivos = {
		"estaciones.txt",
		"rutas.txt",
		"cierres.txt",
		"reportes.txt",
		"recorridos_rutas.txt"
	};

	for (const QString& nombreArchivo : archivos) {
		QString filePath = dir.absoluteFilePath(nombreArchivo);
		QFile file(filePath);

		if (!file.exists()) {
			if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
				QTextStream out(&file);
				out << "# Archivo inicial: " << nombreArchivo << "\n";
				file.close();
			}
		}
	}
}