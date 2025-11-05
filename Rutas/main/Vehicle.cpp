#include "Vehicle.h"

Vehicle::Vehicle() {
    name = "Rustbucket";
    energy = 100;
    speed = 1.0;
    current = nullptr;
}

Vehicle::Vehicle(string name, double energy, double speed, Station* current) {
    this->name = name;
    this->energy = energy;
    this->speed = speed;
    this->current = current;
}

void Vehicle::setCurrentStation(Station* station) {
    current = station;
}

Station* Vehicle::getCurrentStation() {
    return current;
}

void Vehicle::moveTo(Station* destination, double cost) {
    if (energy >= cost) {
        energy -= cost;
        current = destination;
        cout << name << " moved to " << destination->getName() 
             << ". Energy left: " << energy << endl;
    } else {
        cout << name << " does not have enough energy to move!" << endl;
    }
}

void Vehicle::recharge(double amount) {
    energy += amount;
    if (energy > 100) energy = 100;
    cout << name << " recharged. Energy: " << energy << endl;
}

void Vehicle::toString() {
    cout << "Vehicle: " << name 
         << " | Energy: " << energy 
         << " | Speed: " << speed;
    if (current != nullptr)
        cout << " | Current Station: " << current->getName();
    cout << endl;
}
