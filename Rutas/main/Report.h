#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Station.h"
#include "User.h"
using namespace std;

class Report
{
private:
    string title;
    string playerName;
    string characterName;
    vector<string> visited;
    double totalCost;
    string result;

public:
    Report();
    Report(string title, string playerName, string characterName);

    void addVisitedStation(string name);
    void setTotalCost(double cost);
    void setResult(string resultText);
	string getTitle();
	string getPlayer();
	string getCharacter();
	vector<string> getVisitedStations();
	double getTotalCost();
	string getResult();

    void showReport();
    string toString();
};

