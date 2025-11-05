#include "Station.h"

Station::Station() : id(0), name(""), x(0), y(0) {}

Station::Station(int id, string& name, double x, double y)
    : id(id), name(name), x(x), y(y) {}

int Station::getId() {
    return id;
}

std::string Station::getName() {
    return name;
}

double Station::getX() {
    return x;
}

double Station::getY() {
    return y;
}

void Station::toString() {
    cout << "Station [" << id << "] " << name
              << " (" << x << ", " << y << ")" << endl;
}
