#include "Splay.hpp"
#include <algorithm>

uint64_t COMP_COUNT = 0;
uint64_t PTR_COUNT = 0;

Splay::Splay() : root(nullptr), currentHeight(0) {}

Splay::~Splay() {
    freeSubtree(root);
}

void Splay::freeSubtree(SplayNode* node) {
    if (!node) return;
    freeSubtree(node->left);
    freeSubtree(node->right);
    delete node;
}

void Splay::resetCounters() {
    COMP_COUNT = 0;
    PTR_COUNT = 0;
}

void Splay::rotateLeft(SplayNode* x) {
    SplayNode* y = x->right;
    PTR_COUNT += 2; // read x->right and y->left
    x->right = y->left;
    if (y->left) {
        PTR_COUNT += 1;
        y->left->parent = x;
    }
    y->parent = x->parent;
    PTR_COUNT += 1;
    if (!x->parent) {
        root = y;
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

void Splay::rotateRight(SplayNode* x) {
    SplayNode* y = x->left;
    PTR_COUNT += 2; // read x->left and y->right
    x->left = y->right;
    if (y->right) {
        PTR_COUNT += 1;
        y->right->parent = x;
    }
    y->parent = x->parent;
    PTR_COUNT += 1;
    if (!x->parent) {
        root = y;
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

void Splay::splay(SplayNode* x) {
    while (x->parent) {
        if (!x->parent->parent) {
            // zig
            COMP_COUNT += 1;
            if (x == x->parent->left) rotateRight(x->parent);
            else rotateLeft(x->parent);
        } else {
            SplayNode* p = x->parent;
            SplayNode* g = p->parent;
            COMP_COUNT += 1;
            if (x == p->left && p == g->left) {
                // zig-zig
                rotateRight(g);
                rotateRight(p);
            } else if (x == p->right && p == g->right) {
                // zig-zig
                rotateLeft(g);
                rotateLeft(p);
            } else if (x == p->right && p == g->left) {
                // zig-zag
                rotateLeft(p);
                rotateRight(g);
            } else {
                // zig-zag
                rotateRight(p);
                rotateLeft(g);
            }
        }
    }
}

SplayNode* Splay::findNode(int k) {
    SplayNode* x = root;
    while (x) {
        COMP_COUNT += 1;
        if (k < x->key) {
            PTR_COUNT += 1;
            x = x->left;
        } else if (k > x->key) {
            COMP_COUNT += 1;
            PTR_COUNT += 1;
            x = x->right;
        } else {
            break;
        }
    }
    return x;
}

void Splay::insert(int k) {
    SplayNode* z = root;
    SplayNode* p = nullptr;
    while (z) {
        p = z;
        COMP_COUNT += 1;
        if (k < z->key) {
            PTR_COUNT += 1;
            z = z->left;
        } else if (k > z->key) {
            COMP_COUNT += 1;
            PTR_COUNT += 1;
            z = z->right;
        } else {
            // already in tree: splay and return
            splay(z);
            currentHeight = height();
            return;
        }
    }
    SplayNode* node = new SplayNode(k);
    node->parent = p;
    PTR_COUNT += 1;
    if (!p) {
        root = node;
    } else if (k < p->key) {
        PTR_COUNT += 1;
        p->left = node;
    } else {
        PTR_COUNT += 1;
        p->right = node;
    }
    splay(node);
    currentHeight = height();
}

SplayNode* Splay::subtreeMin(SplayNode* x) {
    while (x->left) {
        COMP_COUNT += 1;
        PTR_COUNT += 1;
        x = x->left;
    }
    return x;
}

bool Splay::remove(int k) {
    SplayNode* node = findNode(k);
    if (!node) return false;
    splay(node);
    if (!node->left) {
        root = node->right;
        if (root) root->parent = nullptr;
    } else {
        SplayNode* x = node->right;
        root = node->left;
        root->parent = nullptr;
        SplayNode* m = subtreeMin(root);
        splay(m);
        PTR_COUNT += 1;
        m->right = x;
        if (x) x->parent = m;
    }
    delete node;
    currentHeight = height();
    return true;
}

int Splay::heightRec(SplayNode* node) const {
    if (!node) return 0;
    int lh = heightRec(node->left);
    int rh = heightRec(node->right);
    return std::max(lh, rh) + 1;
}

int Splay::height() const {
    return heightRec(root);
}

void Splay::printTree() const {
    int maxDepth = height();
    std::vector<char> left_trace(maxDepth + 1, ' ');
    std::vector<char> right_trace(maxDepth + 1, ' ');
    printTransposed(root, 0, '-', left_trace, right_trace);
}

void Splay::printTransposed(SplayNode* node,
                            int depth,
                            char prefix,
                            std::vector<char>& left_trace,
                            std::vector<char>& right_trace) {
    if (!node) return;
    if (node->left) {
        printTransposed(node->left, depth + 1, '/', left_trace, right_trace);
    }
    if (prefix == '/') left_trace[depth - 1] = '|';
    if (prefix == '\\') right_trace[depth - 1] = ' ';
    if (depth == 0) std::cout << "-";
    if (depth > 0) std::cout << " ";
    for (int i = 0; i < depth - 1; ++i) {
        if (left_trace[i] == '|' || right_trace[i] == '|') std::cout << "| ";
        else std::cout << "  ";
    }
    if (depth > 0) std::cout << prefix << "-";
    std::cout << "[" << node->key << "]\n";
    left_trace[depth] = ' ';
    if (node->right) {
        right_trace[depth] = '|';
        printTransposed(node->right, depth + 1, '\\', left_trace, right_trace);
    }
}
