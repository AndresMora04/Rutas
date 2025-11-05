#include "User.h"

User::User() {
    username = "Unknown";
    selectedCharacter = "None";
}

User::User(string username, string selectedCharacter) {
    this->username = username;
    this->selectedCharacter = selectedCharacter;
}

string User::getUsername() {
    return username;
}

string User::getSelectedCharacter() {
    return selectedCharacter;
}

void User::setUsername(string username) {
    this->username = username;
}

void User::setSelectedCharacter(string selectedCharacter) {
    this->selectedCharacter = selectedCharacter;
}

void User::toString() {
    cout << "User: " << username 
         << " | Character: " << selectedCharacter << endl;
}