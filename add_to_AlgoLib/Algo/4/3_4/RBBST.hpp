#ifndef RBBST_HPP
#define RBBST_HPP

#include <cstdint>
#include <iostream>
#include <vector>

// Global counters to measure comparisons and pointer operations
extern uint64_t COMP_COUNT;
extern uint64_t PTR_COUNT;

enum Color { RED, BLACK };

struct RBBSTNode {
    int key;
    Color color;
    RBBSTNode* left;
    RBBSTNode* right;
    RBBSTNode* parent;
    RBBSTNode(int k)
        : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBBST {
public:
    RBBST();
    ~RBBST();

    // Insert a key
    void insert(int k);

    // Remove a key; returns true if removed, false if not found
    bool remove(int k);

    // Return tree height
    int height() const;

    // Print tree in a transposed (rotated) form, showing red/black (R/B)
    void printTree() const;

    // Reset counters before each operation (used in experiments)
    void resetCounters();
    // After insert or delete, store tree height here
    int currentHeight;

    // Helper to print nodes with indentation and color
    static void printTransposed(
        RBBSTNode* node,
        RBBSTNode* tnull,
        int depth,
        char prefix,
        std::vector<char>& left_trace,
        std::vector<char>& right_trace
    );

private:
    RBBSTNode* root;
    RBBSTNode* TNULL; // sentinel node, used in place of null pointers

    // Internal helpers:
    void initializeNULLNode(RBBSTNode* node, RBBSTNode* parent);
    void rotateLeft(RBBSTNode*& root, RBBSTNode* x);
    void rotateRight(RBBSTNode*& root, RBBSTNode* x);
    void fixInsert(RBBSTNode*& root, RBBSTNode* k);
    void transplant(RBBSTNode*& u, RBBSTNode*& v);
    RBBSTNode* minimum(RBBSTNode* node) const;
    void fixDelete(RBBSTNode*& root, RBBSTNode* x);
    RBBSTNode* deleteNodeHelper(RBBSTNode*& root, int key, bool& deleted);
    void freeSubtree(RBBSTNode* node);
    int heightRec(RBBSTNode* node) const;
};

#endif // RBBST_HPP
