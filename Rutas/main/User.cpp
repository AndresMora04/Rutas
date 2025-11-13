#include "User.h"

User::User() {
    username = "Desconocido";
    selectedCharacter = "Ninguno";
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
    cout << "Usuario: " << username 
         << " | Personaje: " << selectedCharacter << endl;
}