#pragma once
#include <iostream>
#include <string>
using namespace std;

class User
{
private:
    string username;
    string selectedCharacter;

public:
    User();
    User(string username, string selectedCharacter);

    string getUsername();
    string getSelectedCharacter();

    void setUsername(string username);
    void setSelectedCharacter(string selectedCharacter);

    void toString();
};

