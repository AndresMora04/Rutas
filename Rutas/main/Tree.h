#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <QString>
#include "Station.h"
#include "TreeNode.h"
using namespace std;

class Tree
{
private:
	TreeNode* root;

	void insertNode(TreeNode*& node, Station* s);
	void inOrder(TreeNode* node, vector<string>& result);
	void preOrder(TreeNode* node, vector<string>& result);
	void postOrder(TreeNode* node, vector<string>& result);
	TreeNode* searchNode(TreeNode* node, int id);
	void destroy(TreeNode* node);

public:
	Tree();
	~Tree();

	void insert(Station* s);
	Station* search(int id);

	vector<string> getInOrder();
	vector<string> getPreOrder();
	vector<string> getPostOrder();
	void getAllStations(vector<Station*>& list);
	void collectStations(TreeNode* node, vector<Station*>& list);

	void showInOrder();
	void showPreOrder();
	void showPostOrder();
};

