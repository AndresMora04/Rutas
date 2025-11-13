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
	static vector<Route*> cargarRutas(vector<Station*>& stations);
	static vector<User*> cargarUsuarios();
	static vector<Station*> cargarEstaciones();
	static vector<Report> cargarReportesUsuario(const string& username);
	static vector<Station*> cargarEstacionesUsuario(const string& username);
	static vector<Route*> cargarRutasUsuario(const string& username, vector<Station*>& stations);
	static vector<pair<int, int>> cargarCierresUsuario(const string& username);
	static string obtenerRutaUsuario(const string& username);
	static void guardarEstaciones(vector<Station*>& stations);
	static void guardarRutas(vector<Route*>& routes);
	static void guardarUsuario(User* user);
	static void guardarEstacionesUsuario(const string& username, vector<Station*>& stations);
	static void guardarRutasUsuario(const string& username, vector<Route*>& routes);
	static void guardarCierresUsuario(const string& username, const vector<Route*>& routes);
	static void guardarCierresUsuario(const string& username);
	static void guardarRecorridosUsuario(const string& username,
		const vector<string>& inorder,
		const vector<string>& preorder,
		const vector<string>& postorder);
};
