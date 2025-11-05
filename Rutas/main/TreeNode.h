#pragma once
#include <iostream>
#include "Station.h"
using namespace std;

class TreeNode
{
public:
    Station* station;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Station* s);
};