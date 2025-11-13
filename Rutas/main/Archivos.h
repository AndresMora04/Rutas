#pragma once
#include <vector>
#include "Station.h"
#include "Route.h"
#include <iostream>
#include <fstream>
#include <sstream>  
#include <QDir>
#include "User.h"
#include "Report.h"
using namespace std;

class Archivos
{
public:
	static void guardarEstaciones(vector<Station*>& stations);
	static vector<Station*> cargarEstaciones();
	static void guardarRutas(vector<Route*>& routes);
	static vector<Route*> cargarRutas(std::vector<Station*>& stations);
	static void guardarUsuario(User* user);
	static vector<User*> cargarUsuarios();
	vector<Report> cargarReportesUsuario(const string& username);

	static void guardarEstacionesUsuario(const string& username, vector<Station*>& stations);
	static vector<Station*> cargarEstacionesUsuario(const string& username);
	static void guardarRutasUsuario(const string& username, vector<Route*>& routes);
	static vector<Route*> cargarRutasUsuario(const string& username, vector<Station*>& stations);
	static void guardarRecorridosUsuario(const std::string& username,
		const std::vector<std::string>& inorder,
		const std::vector<std::string>& preorder,
		const std::vector<std::string>& postorder);
	static void guardarCierresUsuario(const string& username, const vector<Route*>& routes);
	static vector<pair<int, int>> cargarCierresUsuario(const string& username);
	void guardarCierresUsuario(const string& username);
	static std::string obtenerRutaUsuario(const std::string& username);
};

