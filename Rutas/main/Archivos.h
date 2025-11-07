#pragma once
#include <vector>
#include "Station.h"
#include "Route.h"
#include <iostream>
#include <fstream>
#include <sstream>  
#include <QDir>
#include "User.h"
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
};

