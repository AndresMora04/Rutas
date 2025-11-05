#include "Tree.h"

Tree::Tree() {
    root = nullptr;
}

void Tree::insertNode(TreeNode*& node, Station* s) {
    if (node == nullptr) {
        node = new TreeNode(s);
    } else if (s->getId() < node->station->getId()) {
        insertNode(node->left, s);
    } else if (s->getId() > node->station->getId()) {
        insertNode(node->right, s);
    }
}

void Tree::insert(Station* s) {
    insertNode(root, s);
}

void Tree::inOrder(TreeNode* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->station->getName() << endl;
        inOrder(node->right);
    }
}

void Tree::preOrder(TreeNode* node) {
    if (node != nullptr) {
        cout << node->station->getName() << endl;
        preOrder(node->left);
        preOrder(node->right);
    }
}

void Tree::postOrder(TreeNode* node) {
    if (node != nullptr) {
        postOrder(node->left);
        postOrder(node->right);
        cout << node->station->getName() << endl;
    }
}

void Tree::showInOrder() {
    cout << "In-Order traversal:" << endl;
    inOrder(root);
}

void Tree::showPreOrder() {
    cout << "Pre-Order traversal:" << endl;
    preOrder(root);
}

void Tree::showPostOrder() {
    cout << "Post-Order traversal:" << endl;
    postOrder(root);
}

TreeNode* Tree::searchNode(TreeNode* node, int id) {
    if (node == nullptr || node->station->getId() == id) {
        return node;
    }
    if (id < node->station->getId()) {
        return searchNode(node->left, id);
    } else {
        return searchNode(node->right, id);
    }
}

Station* Tree::search(int id) {
    TreeNode* found = searchNode(root, id);
    if (found != nullptr) {
        return found->station;
    }
    return nullptr;
}
