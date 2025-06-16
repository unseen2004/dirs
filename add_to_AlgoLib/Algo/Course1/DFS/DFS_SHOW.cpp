
#include <iostream>
#include <vector>

struct Node {
    int val;
    Node* left;
    Node* right;

    Node(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Function to perform pre-order traversal
std::vector<int> walk(Node* node, std::vector<int>& path) {
    if (!node) {
        return path;
    }

    // Pre-order: Visit the current node first
    path.push_back(node->val);

    walk(node->left, path);  // Traverse left subtree
    walk(node->right, path); // Traverse right subtree

    return path;
}

std::vector<int> pre_order_search(Node* head) {
    std::vector<int> path;  // Initialize an empty path vector
    return walk(head, path);  // Perform pre-order traversal
}

int main() {
    // create tree
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->right = new Node(6);

    // Perform pre-order search
    std::vector<int> result = pre_order_search(root);

    // Print the result
    std::cout << "Pre-order traversal: ";
    for (int val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Clean up memory
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right->right;
    delete root->right;
    delete root;

    return 0;
}
