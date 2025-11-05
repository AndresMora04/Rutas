#include "Randomizer.h"

Randomizer::Randomizer() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

void Randomizer::resetRandom() {
    srand(time(0));
}

Event* Randomizer::generateRandomEvent(vector<Route*>& routes) {
    if (routes.empty()) return nullptr;

    int randomIndex = rand() % routes.size();
    Route* selectedRoute = routes[randomIndex];

    int chance = rand() % 2;

    if (chance == 1 && !selectedRoute->isClosed()) {
        selectedRoute->setClosed(true);

        string type = "Alien Attack";
        string description = "An alien attack has closed the route!";
        Station* location = selectedRoute->getStart();

        Event* e = new Event(type, description, location);
        cout << "Random Event: " << type 
             << " | Location: " << location->getName() << endl;

        return e;
    }

    return nullptr;
}