#include <iostream>
#include <vector>
#include <algorithm>
#include "Splay.hpp"
#include "Random.hpp"

int main() {
    const int n = 30;
    Splay tree;

    std::cout << "=== Splay Demo: Case 1 (Insert 1.." << n << ", delete random) ===\n";
    // Insert 1..n in increasing order
    for (int i = 1; i <= n; ++i) {
        std::cout << "insert " << i << "\n";
        tree.insert(i);
        tree.printTree();
        std::cout << "\n";
    }
    // Delete them in a random permutation
    std::vector<int> perm1(n);
    for (int i = 0; i < n; ++i) perm1[i] = i + 1;
    std::shuffle(perm1.begin(), perm1.end(), Random::mt);
    for (int k : perm1) {
        std::cout << "delete " << k << "\n";
        tree.remove(k);
        tree.printTree();
        std::cout << "\n";
    }

    // Case 2: insert random permutation, then delete random
    Splay tree2;
    std::cout << "=== Splay Demo: Case 2 (Insert random 1.." << n << ", delete random) ===\n";
    std::vector<int> perm2(n);
    for (int i = 0; i < n; ++i) perm2[i] = i + 1;
    std::shuffle(perm2.begin(), perm2.end(), Random::mt);
    for (int k : perm2) {
        std::cout << "insert " << k << "\n";
        tree2.insert(k);
        tree2.printTree();
        std::cout << "\n";
    }
    std::vector<int> perm3 = perm2;
    std::shuffle(perm3.begin(), perm3.end(), Random::mt);
    for (int k : perm3) {
        std::cout << "delete " << k << "\n";
        tree2.remove(k);
        tree2.printTree();
        std::cout << "\n";
    }

    return 0;
}
