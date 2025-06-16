#ifndef BST_HPP
#define BST_HPP

#include <cstdint>
#include <iostream>
#include <vector>

// Counters for experiments
extern uint64_t COMP_COUNT;
extern uint64_t PTR_COUNT;

struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
public:
    BST();
    ~BST();

    // Insert a key
    void insert(int k);

    // Remove a key; returns true if removed, false if not found
    bool remove(int k);

    // Return current tree height
    int height() const;

    // Print tree in transposed form (rotated)
    void printTree() const;

    // Reset the comparison/pointer counters (for experiments)
    void resetCounters();

    // After each operation, store the tree’s height here
    int currentHeight;

    // Helper for printing (transposed)
    static void printTransposed(
        BSTNode* node,
        int depth,
        char prefix,
        std::vector<char>& left_trace,
        std::vector<char>& right_trace
    );

private:
    BSTNode* root;

    // Internal recursive helpers
    BSTNode* insertNode(BSTNode* node, int k);
    BSTNode* deleteNode(BSTNode* node, int k, bool& deleted);
    BSTNode* findMin(BSTNode* node);
    void freeSubtree(BSTNode* node);
    int heightRec(BSTNode* node) const;
};

#endif // BST_HPP
