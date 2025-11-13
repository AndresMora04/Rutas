#include "SessionManager.h"

User SessionManager::currentUser;
bool SessionManager::loggedIn = false;

SessionManager::SessionManager() {}

void SessionManager::login(string username, string character) {
    currentUser = User(username, character);
    loggedIn = true;
    cout << "Usuario conectado: " << username 
         << " | Bus: " << character << endl;
}

User SessionManager::getCurrentUser() {
    return currentUser;
}

bool SessionManager::isLoggedIn() {
    return loggedIn;
}

void SessionManager::logout() {
    cout << "Usuario desconectado: " << currentUser.getUsername() << endl;
    loggedIn = false;
}