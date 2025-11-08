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
	string type;

public:
	Station();
	Station(int id, string name, double x, double y, string type);
	int getId();
	string getName();
	double getX();
	double getY();
	string getType();
	void setType(string type);
	void toString();
};

