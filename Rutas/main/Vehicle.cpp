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
    } 
}

void Vehicle::recharge(double amount) {
    energy += amount;
    if (energy > 100) energy = 100;
}

void Vehicle::toString() {
    cout << "Bus: " << name 
         << " | Energia: " << energy 
         << " | Velocidad: " << speed;
    if (current != nullptr)
        cout << " | Estacion Actual: " << current->getName();
    cout << endl;
}
