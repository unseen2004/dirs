#include "BST.hpp"
#include <algorithm>
#include <iostream>

uint64_t COMP_COUNT = 0;
uint64_t PTR_COUNT = 0;

BST::BST() : root(nullptr), currentHeight(0) {}

BST::~BST() {
    freeSubtree(root);
}

void BST::freeSubtree(BSTNode* node) {
    if (!node) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}

void BST::resetCounters() {
    COMP_COUNT = 0;
    PTR_COUNT = 0;
}

void BST::insert(int k) {
    root = insertNode(root, k);
    currentHeight = height();
}

BSTNode* BST::insertNode(BSTNode* node, int k) {
    if (!node) {
        PTR_COUNT += 1; // linking new node
        return new BSTNode(k);
    }
    COMP_COUNT += 1;
    if (k < node->key) {
        PTR_COUNT += 1; // traversing pointer
        node->left = insertNode(node->left, k);
    } else {
        PTR_COUNT += 1;
        node->right = insertNode(node->right, k);
    }
    return node;
}

bool BST::remove(int k) {
    bool deleted = false;
    root = deleteNode(root, k, deleted);
    if (deleted) currentHeight = height();
    return deleted;
}

BSTNode* BST::findMin(BSTNode* node) {
    while (node->left) {
        COMP_COUNT += 1;
        PTR_COUNT += 1;
        node = node->left;
    }
    return node;
}

BSTNode* BST::deleteNode(BSTNode* node, int k, bool& deleted) {
    if (!node) return nullptr;
    COMP_COUNT += 1;
    if (k < node->key) {
        PTR_COUNT += 1;
        node->left = deleteNode(node->left, k, deleted);
    } else if (k > node->key) {
        COMP_COUNT += 1;
        PTR_COUNT += 1;
        node->right = deleteNode(node->right, k, deleted);
    } else {
        // found node to delete
        deleted = true;
        if (!node->left) {
            BSTNode* temp = node->right;
            PTR_COUNT += 1;
            delete node;
            return temp;
        } else if (!node->right) {
            BSTNode* temp = node->left;
            PTR_COUNT += 1;
            delete node;
            return temp;
        } else {
            BSTNode* succ = findMin(node->right);
            COMP_COUNT += 1;
            node->key = succ->key;
            PTR_COUNT += 1;
            node->right = deleteNode(node->right, succ->key, deleted);
        }
    }
    return node;
}

int BST::heightRec(BSTNode* node) const {
    if (!node) return 0;
    int lh = heightRec(node->left);
    int rh = heightRec(node->right);
    return std::max(lh, rh) + 1;
}

int BST::height() const {
    return heightRec(root);
}

void BST::printTree() const {
    int maxDepth = height();
    std::vector<char> left_trace(maxDepth + 1, ' ');
    std::vector<char> right_trace(maxDepth + 1, ' ');
    printTransposed(root, 0, '-', left_trace, right_trace);
}

void BST::printTransposed(
    BSTNode* node,
    int depth,
    char prefix,
    std::vector<char>& left_trace,
    std::vector<char>& right_trace
) {
    if (!node) return;
    if (node->left) {
        printTransposed(node->left, depth + 1, '/', left_trace, right_trace);
    }
    if (prefix == '/') left_trace[depth - 1] = '|';
    if (prefix == '\\') right_trace[depth - 1] = ' ';
    if (depth == 0) std::cout << "-";
    if (depth > 0) std::cout << " ";
    for (int i = 0; i < depth - 1; ++i) {
        if (left_trace[i] == '|' || right_trace[i] == '|')
            std::cout << "| ";
        else
            std::cout << "  ";
    }
    if (depth > 0) std::cout << prefix << "-";
    std::cout << "[" << node->key << "]\n";
    left_trace[depth] = ' ';
    if (node->right) {
        right_trace[depth] = '|';
        printTransposed(node->right, depth + 1, '\\', left_trace, right_trace);
    }
}
