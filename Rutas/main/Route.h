#pragma once
#include <iostream>
#include <string>
#include "Station.h"

class Route
{
private:
    Station* start;
    Station* end;
    double cost;
    bool closed;

public:
    Route();
    Route(Station* start, Station* end, double cost, bool closed);

    Station* getStart();
    Station* getEnd();
    double getCost();
    bool isClosed();
	void setCost(double newCost);

    void setClosed(bool value);
    void toString();
	QString getDisplayText() const;
};

