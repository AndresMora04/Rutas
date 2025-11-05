#pragma once
#include <iostream>
#include <string>
#include "User.h"
using namespace std;

class SessionManager
{
private:
    static User currentUser;
    static bool loggedIn;

public:
    SessionManager();

    static void login(string username, string character);
    static User getCurrentUser();
    static bool isLoggedIn();
    static void logout();
};

