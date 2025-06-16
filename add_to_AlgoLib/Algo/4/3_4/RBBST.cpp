#include "RBBST.hpp"
#include <algorithm>
#include <iostream>

// Define the global counters
uint64_t COMP_COUNT = 0;
uint64_t PTR_COUNT = 0;

RBBST::RBBST() {
    // Create the TNULL sentinel node
    TNULL = new RBBSTNode(0);
    TNULL->color = BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
    currentHeight = 0;
}

RBBST::~RBBST() {
    // Free all nodes (post-order) and then delete TNULL
    freeSubtree(root);
    delete TNULL;
}

void RBBST::initializeNULLNode(RBBSTNode* node, RBBSTNode* parent) {
    node->key = 0;
    node->color = BLACK;
    node->left = nullptr;
    node->right = nullptr;
    node->parent = parent;
}

void RBBST::rotateLeft(RBBSTNode*& r, RBBSTNode* x) {
    RBBSTNode* y = x->right;
    PTR_COUNT += 1;
    x->right = y->left;
    PTR_COUNT += 1;
    if (y->left != TNULL) {
        PTR_COUNT += 1;
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        r = y;
    } else if (x == x->parent->left) {
        PTR_COUNT += 1;
        x->parent->left = y;
    } else {
        PTR_COUNT += 1;
        x->parent->right = y;
    }
    y->left = x;
    PTR_COUNT += 1;
    x->parent = y;
    PTR_COUNT += 1;
}

void RBBST::rotateRight(RBBSTNode*& r, RBBSTNode* x) {
    RBBSTNode* y = x->left;
    PTR_COUNT += 1;
    x->left = y->right;
    PTR_COUNT += 1;
    if (y->right != TNULL) {
        PTR_COUNT += 1;
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        r = y;
    } else if (x == x->parent->right) {
        PTR_COUNT += 1;
        x->parent->right = y;
    } else {
        PTR_COUNT += 1;
        x->parent->left = y;
    }
    y->right = x;
    PTR_COUNT += 1;
    x->parent = y;
    PTR_COUNT += 1;
}

void RBBST::fixInsert(RBBSTNode*& r, RBBSTNode* k) {
    RBBSTNode* u;
    while (k->parent->color == RED) {
        COMP_COUNT += 1;
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u->color == RED) {
                // Case 1
                COMP_COUNT += 1;
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    // Case 2
                    k = k->parent;
                    rotateRight(r, k);
                }
                // Case 3
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rotateLeft(r, k->parent->parent);
            }
        } else {
            // Mirror case
            u = k->parent->parent->right;
            if (u->color == RED) {
                COMP_COUNT += 1;
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    rotateLeft(r, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rotateRight(r, k->parent->parent);
            }
        }
        if (k == r) break;
    }
    r->color = BLACK;
}

void RBBST::insert(int key) {
    // Create new node (initially RED)
    RBBSTNode* node = new RBBSTNode(key);
    node->parent = nullptr;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;

    RBBSTNode* y = nullptr;
    RBBSTNode* x = this->root;

    // Standard BST insertion
    while (x != TNULL) {
        y = x;
        COMP_COUNT += 1;
        if (node->key < x->key) {
            PTR_COUNT += 1;
            x = x->left;
        } else {
            PTR_COUNT += 1;
            x = x->right;
        }
    }
    node->parent = y;
    if (y == nullptr) {
        root = node;
    } else if (node->key < y->key) {
        PTR_COUNT += 1;
        y->left = node;
    } else {
        PTR_COUNT += 1;
        y->right = node;
    }

    // Fix possible violations
    if (node->parent == nullptr) {
        // Node is root
        node->color = BLACK;
        currentHeight = height();
        return;
    }
    if (node->parent->parent == nullptr) {
        currentHeight = height();
        return;
    }
    fixInsert(root, node);
    currentHeight = height();
}

RBBSTNode* RBBST::minimum(RBBSTNode* node) const {
    while (node->left != TNULL) {
        COMP_COUNT += 1;
        PTR_COUNT += 1;
        node = node->left;
    }
    return node;
}

void RBBST::transplant(RBBSTNode*& u, RBBSTNode*& v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        PTR_COUNT += 1;
        u->parent->left = v;
    } else {
        PTR_COUNT += 1;
        u->parent->right = v;
    }
    v->parent = u->parent;
    PTR_COUNT += 1;
}

RBBSTNode* RBBST::deleteNodeHelper(RBBSTNode*& r, int key, bool& deleted) {
    RBBSTNode* z = TNULL;
    RBBSTNode* x, *y;
    RBBSTNode* node = r;
    while (node != TNULL) {
        COMP_COUNT += 1;
        if (node->key == key) {
            z = node;
        }
        if (node->key <= key) {
            PTR_COUNT += 1;
            node = node->right;
        } else {
            PTR_COUNT += 1;
            node = node->left;
        }
    }
    if (z == TNULL) return r;  // Key not found
    deleted = true;
    y = z;
    Color y_original_color = y->color;
    if (z->left == TNULL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            PTR_COUNT += 1;
            y->right->parent = y;
            PTR_COUNT += 1;
        }
        transplant(z, y);
        y->left = z->left;
        PTR_COUNT += 1;
        y->left->parent = y;
        PTR_COUNT += 1;
        y->color = z->color;
        PTR_COUNT += 1;
    }
    delete z;
    if (y_original_color == BLACK) {
        fixDelete(r, x);
    }
    return r;
}

void RBBST::fixDelete(RBBSTNode*& r, RBBSTNode* x) {
    RBBSTNode* s;
    while (x != r && x->color == BLACK) {
        COMP_COUNT += 1;
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                COMP_COUNT += 1;
                s->color = BLACK;
                x->parent->color = RED;
                rotateLeft(r, x->parent);
                s = x->parent->right;
            }
            if (s->left->color == BLACK && s->right->color == BLACK) {
                COMP_COUNT += 2;
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    COMP_COUNT += 1;
                    s->left->color = BLACK;
                    s->color = RED;
                    rotateRight(r, s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                rotateLeft(r, x->parent);
                x = r;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                COMP_COUNT += 1;
                s->color = BLACK;
                x->parent->color = RED;
                rotateRight(r, x->parent);
                s = x->parent->left;
            }
            if (s->right->color == BLACK && s->left->color == BLACK) {
                COMP_COUNT += 2;
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    COMP_COUNT += 1;
                    s->right->color = BLACK;
                    s->color = RED;
                    rotateLeft(r, s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rotateRight(r, x->parent);
                x = r;
            }
        }
    }
    x->color = BLACK;
}

bool RBBST::remove(int key) {
    bool deleted = false;
    root = deleteNodeHelper(root, key, deleted);
    if (deleted) currentHeight = height();
    return deleted;
}

int RBBST::heightRec(RBBSTNode* node) const {
    if (node == TNULL) return 0;
    int lh = heightRec(node->left);
    int rh = heightRec(node->right);
    return std::max(lh, rh) + 1;
}

int RBBST::height() const {
    return heightRec(root);
}

void RBBST::printTree() const {
    int maxDepth = height();
    std::vector<char> left_trace(maxDepth + 1, ' ');
    std::vector<char> right_trace(maxDepth + 1, ' ');
    printTransposed(root, TNULL, 0, '-', left_trace, right_trace);  // Pass TNULL here
}

void RBBST::printTransposed(RBBSTNode* node,
                           RBBSTNode* tnull,  // Added parameter
                           int depth,
                           char prefix,
                           std::vector<char>& left_trace,
                           std::vector<char>& right_trace) {
    if (!node || node == tnull) return;
    if (node->left && node->left != tnull) {
        printTransposed(node->left, tnull, depth + 1, '/', left_trace, right_trace);
    }
    // Rest of the function remains the same

    // Replace this line too
    if (node->right && node->right != tnull) {
        right_trace[depth] = '|';
        printTransposed(node->right, tnull, depth + 1, '\\', left_trace, right_trace);
    }
}

void RBBST::resetCounters() {
    COMP_COUNT = 0;
    PTR_COUNT = 0;
}

void RBBST::freeSubtree(RBBSTNode* node) {
    if (!node || node == TNULL) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}
