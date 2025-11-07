#pragma once
#include <QString>
#include <QPointF>
#include "Station.h"
#include "Route.h"
#include <iostream>
using namespace std;

class Utils
{
public:
    static Station* crearEstacion(int id, string& name, QPointF pos);
    static Route* crearRuta(Station* start, Station* end);
    static double calcularDistancia(Station* a, Station* b);
};

