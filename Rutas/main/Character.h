#pragma once
#include <iostream>
#include <string>
using namespace std;

class Character
{
private:
    string name;
    string description;
    string imagePath;
    double speed;
    double energy;

public:
    Character();
    Character(string name, string description, string imagePath, double speed, double energy);

    string getName();
    string getDescription();
    string getImagePath();
    double getSpeed();
    double getEnergy();

    void toString();
};

