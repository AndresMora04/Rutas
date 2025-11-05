#pragma once
#include <string>
#include <iostream>
using namespace std;

class Station
{
private:
    int id;
    string name;
    double x;
    double y;

public:
    Station();
    Station(int id, string& name, double x, double y);

    int getId();
    string getName();
    double getX();
    double getY();

    void toString();
};

