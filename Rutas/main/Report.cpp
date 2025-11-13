#include "Report.h"

Report::Report() {
    title = "Unnamed Report";
    playerName = "Unknown";
    characterName = "None";
    totalCost = 0;
    result = "No result";
}

Report::Report(string title, string playerName, string characterName) {
    this->title = title;
    this->playerName = playerName;
    this->characterName = characterName;
    totalCost = 0;
    result = "";
}

void Report::addVisitedStation(string name) {
    visited.push_back(name);
}

void Report::setTotalCost(double cost) {
    totalCost = cost;
}

void Report::setResult(string resultText) {
    result = resultText;
}

void Report::showReport() {
    cout << "----- REPORT -----" << endl;
    cout << "Title: " << title << endl;
    cout << "Player: " << playerName << " | Character: " << characterName << endl;
    cout << "Visited Stations: ";
    for (int i = 0; i < visited.size(); i++) {
        cout << visited[i];
        if (i < visited.size() - 1) cout << " -> ";
    }
    cout << endl;
    cout << "Total Cost: " << totalCost << endl;
    cout << "Result: " << result << endl;
    cout << "------------------" << endl;
}

string Report::toString() {
    string text = "REPORT: " + title + "\n";
    text += "Player: " + playerName + " | Character: " + characterName + "\n";
    text += "Visited: ";
    for (int i = 0; i < visited.size(); i++) {
        text += visited[i];
        if (i < visited.size() - 1) text += " -> ";
    }
    text += "\nTotal Cost: " + to_string(totalCost) + "\n";
    text += "Result: " + result + "\n";
    text += "---------------------------\n";
    return text;
}

string Report::getTitle() {
    return title;
}

string Report::getPlayer() {
    return playerName;
}   

string Report::getCharacter() {
    return characterName;
}

vector<string> Report::getVisitedStations() {
    return visited;
}

double Report::getTotalCost() {
    return totalCost;
}

string Report::getResult() {
    return result;
}

