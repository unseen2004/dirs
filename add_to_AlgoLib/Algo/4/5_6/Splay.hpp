#ifndef SPLAY_HPP
#define SPLAY_HPP

#include <cstdint>
#include <iostream>
#include <vector>

// Counters for experiments
extern uint64_t COMP_COUNT;
extern uint64_t PTR_COUNT;

struct SplayNode {
    int key;
    SplayNode* left;
    SplayNode* right;
    SplayNode* parent;
    SplayNode(int k) : key(k), left(nullptr), right(nullptr), parent(nullptr) {}
};

class Splay {
public:
    Splay();
    ~Splay();

    void insert(int k);
    bool remove(int k);
    int height() const;
    void printTree() const;

    void resetCounters();
    int currentHeight;

    static void printTransposed(SplayNode* node,
                                int depth,
                                char prefix,
                                std::vector<char>& left_trace,
                                std::vector<char>& right_trace);
private:
    SplayNode* root;

    void freeSubtree(SplayNode* node);
    void rotateLeft(SplayNode* x);
    void rotateRight(SplayNode* x);
    void splay(SplayNode* x);
    SplayNode* subtreeMin(SplayNode* x);
    SplayNode* findNode(int k);
    int heightRec(SplayNode* node) const;
};

#endif // SPLAY_HPP
