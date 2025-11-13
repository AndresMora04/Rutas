#include "Event.h"

Event::Event() {
    type = "Desconocido";
    description = "No detalles";
    location = nullptr;
    active = false;
}

Event::Event(string type, string description, Station* location) {
    this->type = type;
    this->description = description;
    this->location = location;
    this->active = true;
}

string Event::getType() {
    return type;
}

string Event::getDescription() {
    return description;
}

Station* Event::getLocation() {
    return location;
}

bool Event::isActive() {
    return active;
}

void Event::resolve() {
    active = false;
    cout << "Evento resuelto en la estacion " << location->getName() << endl;
}

void Event::toString() {
    cout << "Tipo de Evento: " << type 
         << " | Descripcion: " << description;
    if (location != nullptr)
        cout << " | Ubicacion: " << location->getName();
    cout << " | Activo: " << (active ? "Si" : "No") << endl;
}
