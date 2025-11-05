#include "Event.h"

Event::Event() {
    type = "Unknown";
    description = "No details";
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
    cout << "Event resolved at station: " << location->getName() << endl;
}

void Event::toString() {
    cout << "Event Type: " << type 
         << " | Description: " << description;
    if (location != nullptr)
        cout << " | Location: " << location->getName();
    cout << " | Active: " << (active ? "Yes" : "No") << endl;
}
