#include "Character.h"

Character::Character() {
    name = "Desconocido";
    description = "No descripcion";
    imagePath = "";
    speed = 1.0;
    energy = 100;
}

Character::Character(string name, string description, string imagePath, double speed, double energy) {
    this->name = name;
    this->description = description;
    this->imagePath = imagePath;
    this->speed = speed;
    this->energy = energy;
}

string Character::getName() {
    return name;
}

string Character::getDescription() {
    return description;
}

string Character::getImagePath() {
    return imagePath;
}

double Character::getSpeed() {
    return speed;
}

double Character::getEnergy() {
    return energy;
}

void Character::toString() {
    cout << "Bus: " << name
         << " | Velocidad: " << speed
         << " | Energía: " << energy << endl;
}
