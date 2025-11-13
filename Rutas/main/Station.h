#pragma once
#include <string>
#include <iostream>
#include <QString>
using namespace std;

class Station
{
private:
	int id;
	string name;
	double x;
	double y;
	string type;
	bool blocked = false;

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
	bool isBlocked();	
	void setBlocked(bool value);
	QString getDisplayText() const;
};

