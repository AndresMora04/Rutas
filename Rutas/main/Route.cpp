#include "Route.h"

Route::Route() {
    start = nullptr;
    end = nullptr;
    cost = 0;
    closed = false;
}

Route::Route(Station* start, Station* end, double cost, bool closed) {
    this->start = start;
    this->end = end;
    this->cost = cost;
    this->closed = closed;
}

Station* Route::getStart() {
    return start;
}

Station* Route::getEnd() {
    return end;
}

double Route::getCost() {
    return cost;
}

bool Route::isClosed() {
    return closed;
}

void Route::setClosed(bool value) {
    closed = value;
}

void Route::toString() {
    cout << "Route: " << start->getName() << " -> " << end->getName()
         << " | Cost: " << cost
         << " | Closed: " << (closed ? "Yes" : "No") << endl;
}