#pragma once
#include <iostream>
#include <string>
#include "Station.h"
using namespace std;

class Vehicle
{
private:
    string name;
    double energy;
    Station* current;
    double speed;

public:
    Vehicle();
    Vehicle(string name, double energy, double speed, Station* current);

    void setCurrentStation(Station* station);
    Station* getCurrentStation();

    void moveTo(Station* destination, double cost);
    void recharge(double amount);
    void toString();
};

