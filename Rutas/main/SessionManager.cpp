#include "SessionManager.h"

User SessionManager::currentUser;
bool SessionManager::loggedIn = false;

SessionManager::SessionManager() {}

void SessionManager::login(string username, string character) {
    currentUser = User(username, character);
    loggedIn = true;
    cout << "User logged in: " << username 
         << " | Character: " << character << endl;
}

User SessionManager::getCurrentUser() {
    return currentUser;
}

bool SessionManager::isLoggedIn() {
    return loggedIn;
}

void SessionManager::logout() {
    cout << "User logged out: " << currentUser.getUsername() << endl;
    loggedIn = false;
}