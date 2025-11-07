#include "Utils.h"

Station* Utils::crearEstacion(int id, string& name, QPointF pos)
{
    Station* s = new Station(id, name, pos.x(), pos.y());
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