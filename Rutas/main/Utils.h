#pragma once
#include <QString>
#include <QPointF>
#include "Station.h"
#include "Route.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;

class Utils
{
public:
	static Station* crearEstacion(int id, string& name, QPointF pos, string type);
    static Route* crearRuta(Station* start, Station* end);
    static double calcularDistancia(Station* a, Station* b);
	static int getNextStationId();
};

