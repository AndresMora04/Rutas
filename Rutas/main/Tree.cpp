#include "Tree.h"

Tree::Tree() {
	root = nullptr;
}

Tree::~Tree() {
	destroy(root);
}

void Tree::destroy(TreeNode* node) {
	if (node) {
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
}

void Tree::insertNode(TreeNode*& node, Station* s) {
	if (node == nullptr)
		node = new TreeNode(s);
	else if (s->getId() < node->station->getId())
		insertNode(node->left, s);
	else if (s->getId() > node->station->getId())
		insertNode(node->right, s);
}

void Tree::insert(Station* s) {
	insertNode(root, s);
}

void Tree::inOrder(TreeNode* node, vector<string>& result) {
	if (node) {
		inOrder(node->left, result);
		result.push_back(node->station->getName());
		inOrder(node->right, result);
	}
}

void Tree::preOrder(TreeNode* node, vector<string>& result) {
	if (node) {
		result.push_back(node->station->getName());
		preOrder(node->left, result);
		preOrder(node->right, result);
	}
}

void Tree::postOrder(TreeNode* node, vector<string>& result) {
	if (node) {
		postOrder(node->left, result);
		postOrder(node->right, result);
		result.push_back(node->station->getName());
	}
}

vector<string> Tree::getInOrder() {
	vector<string> result;
	inOrder(root, result);
	return result;
}

vector<string> Tree::getPreOrder() {
	vector<string> result;
	preOrder(root, result);
	return result;
}

vector<string> Tree::getPostOrder() {
	vector<string> result;
	postOrder(root, result);
	return result;
}

void Tree::collectStations(TreeNode* node, vector<Station*>& list) {
	if (node != nullptr) {
		collectStations(node->left, list);
		list.push_back(node->station);
		collectStations(node->right, list);
	}
}

void Tree::getAllStations(vector<Station*>& list) {
	collectStations(root, list);
}

TreeNode* Tree::searchNode(TreeNode* node, int id) {
	if (!node) return nullptr;
	if (node->station->getId() == id)
		return node;
	if (id < node->station->getId())
		return searchNode(node->left, id);
	return searchNode(node->right, id);
}

Station* Tree::search(int id) {
	TreeNode* found = searchNode(root, id);
	return found ? found->station : nullptr;
}

void Tree::showInOrder() {
	vector<string> res = getInOrder();
	cout << "InOrder: ";
	for (auto& name : res) cout << name << " ";
	cout << endl;
}

void Tree::showPreOrder() {
	vector<string> res = getPreOrder();
	cout << "PreOrder: ";
	for (auto& name : res) cout << name << " ";
	cout << endl;
}

void Tree::showPostOrder() {
	vector<string> res = getPostOrder();
	cout << "PostOrder: ";
	for (auto& name : res) cout << name << " ";
	cout << endl;
}

Station* Tree::searchByName(const string& name) {
    return searchByNameRecursive(root, name);
}

Station* Tree::searchByNameRecursive(TreeNode* node, const string& name) {
    if (!node) return nullptr;
    if (node->station->getName() == name)
        return node->station;
    Station* left = searchByNameRecursive(node->left, name);
    if (left) return left;
    return searchByNameRecursive(node->right, name);
}

bool Tree::deleteByName(const string& name) {
    return deleteByNameRecursive(root, name);
}

bool Tree::deleteByNameRecursive(TreeNode*& node, const string& name) {
    if (!node) return false;

    if (node->station->getName() == name) {
        if (!node->left && !node->right) {
            delete node;
            node = nullptr;
        }
        else if (!node->left) {
            TreeNode* temp = node;
            node = node->right;
            delete temp;
        }
        else if (!node->right) {
            TreeNode* temp = node;
            node = node->left;
            delete temp;
        }
        else {
            TreeNode* successor = node->right;
            while (successor->left)
                successor = successor->left;
            node->station = successor->station;
            deleteByNameRecursive(node->right, successor->station->getName());
        }
        return true;
    }

    return deleteByNameRecursive(node->left, name) || deleteByNameRecursive(node->right, name);
}