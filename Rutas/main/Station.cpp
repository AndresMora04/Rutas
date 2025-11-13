#include "Station.h"

Station::Station() : id(0), name(""), x(0), y(0), type("Stop") {}

Station::Station(int id, string name, double x, double y, string type)
    : id(id), name(name), x(x), y(y), type(type) {}

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

string Station::getType() {
	return type;
}

void Station::setType(string type) {
	this->type = type;
}

void Station::toString() {
	cout << "Station [" << id << "] " << name
		<< " (" << x << ", " << y << ")" << endl;
}

bool Station::isBlocked()
{
	return false;
}

void Station::setBlocked(bool value)
{
	blocked = value;
}

QString Station::getDisplayText() const {
    return QString::fromUtf8(name.c_str()) + " (" + QString::fromUtf8(type.c_str()) + ")";
}