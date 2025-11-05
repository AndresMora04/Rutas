#pragma once
#include <iostream>
#include "Station.h"
#include "TreeNode.h"
using namespace std;

class Tree
{
private:
    TreeNode* root;

    void insertNode(TreeNode*& node, Station* s);
    void inOrder(TreeNode* node);
    void preOrder(TreeNode* node);
    void postOrder(TreeNode* node);
    TreeNode* searchNode(TreeNode* node, int id);

public:
    Tree();

    void insert(Station* s);
    void showInOrder();
    void showPreOrder();
    void showPostOrder();
    Station* search(int id);
};

