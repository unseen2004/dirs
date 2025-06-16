#include <iostream>
using namespace std;

class AVLTree {
private:
    struct Node {
        int m_data;
        Node* m_left;
        Node* m_right;
        int m_height;

        Node(int data) : m_data(data), m_left(nullptr), m_right(nullptr), m_height(1) {}
    };

    Node* m_root;

public:
    // Constructor
    AVLTree() : m_root(nullptr) {}

    // Destructor
    ~AVLTree() {
        clear(m_root);
    }

    // Insert a new value into the AVL tree
    void insert(int value) {
        m_root = insert(m_root, value);
    }

    // Remove a value from the AVL tree
    bool remove(int value) {
        if (contains(value)) {
            m_root = remove(m_root, value);
            return true;
        }
        return false;
    }

    // Check if a value exists in the AVL tree
    bool contains(int value) const {
        return contains(m_root, value);
    }

    // Get the height of the AVL tree
    int getHeight() const {
        return getHeight(m_root);
    }

    // Print the AVL tree in-order
    void printInOrder() const {
        printInOrder(m_root);
        cout << endl;
    }

private:
    // Helper function for inserting a value
    Node* insert(Node* node, int value) {
        if (!node) return new Node(value);

        if (value < node->m_data) {
            node->m_left = insert(node->m_left, value);
        } else if (value > node->m_data) {
            node->m_right = insert(node->m_right, value);
        } else {
            return node;  // Duplicate values are not allowed
        }

        node->m_height = 1 + max(getHeight(node->m_left), getHeight(node->m_right));

        return balance(node);
    }

    // Helper function for removing a value
    Node* remove(Node* node, int value) {
        if (!node) return nullptr;

        if (value < node->m_data) {
            node->m_left = remove(node->m_left, value);
        } else if (value > node->m_data) {
            node->m_right = remove(node->m_right, value);
        } else {
            if (!node->m_left || !node->m_right) {
                Node* temp = node->m_left ? node->m_left : node->m_right;
                delete node;
                return temp;
            }

            Node* minNode = getMinNode(node->m_right);
            node->m_data = minNode->m_data;
            node->m_right = remove(node->m_right, minNode->m_data);
        }

        node->m_height = 1 + max(getHeight(node->m_left), getHeight(node->m_right));
        return balance(node);
    }

    // Helper function for checking if a value exists
    bool contains(Node* node, int value) const {
        if (!node) return false;

        if (value < node->m_data) {
            return contains(node->m_left, value);
        } else if (value > node->m_data) {
            return contains(node->m_right, value);
        }
        return true;
    }

    // Balancing functions
    Node* balance(Node* node) {
        int balanceFactor = getBalanceFactor(node);

        // Left-heavy case
        if (balanceFactor < -1) {
            if (getBalanceFactor(node->m_left) > 0) {
                node->m_left = rotateLeft(node->m_left);
            }
            return rotateRight(node);
        }

        // Right-heavy case
        if (balanceFactor > 1) {
            if (getBalanceFactor(node->m_right) < 0) {
                node->m_right = rotateRight(node->m_right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    int getBalanceFactor(Node* node) const {
        if (!node) return 0;
        return getHeight(node->m_right) - getHeight(node->m_left);
    }

    // Rotation functions
    Node* rotateLeft(Node* node) {
        Node* newRoot = node->m_right;
        node->m_right = newRoot->m_left;
        newRoot->m_left = node;

        node->m_height = 1 + max(getHeight(node->m_left), getHeight(node->m_right));
        newRoot->m_height = 1 + max(getHeight(newRoot->m_left), getHeight(newRoot->m_right));

        return newRoot;
    }

    Node* rotateRight(Node* node) {
        Node* newRoot = node->m_left;
        node->m_left = newRoot->m_right;
        newRoot->m_right = node;

        node->m_height = 1 + max(getHeight(node->m_left), getHeight(node->m_right));
        newRoot->m_height = 1 + max(getHeight(newRoot->m_left), getHeight(newRoot->m_right));

        return newRoot;
    }

    // Utility functions
    int getHeight(Node* node) const {
        return node ? node->m_height : 0;
    }

    int max(int a, int b) const {
        return (a > b) ? a : b;
    }

    void printInOrder(Node* node) const {
        if (!node) return;

        printInOrder(node->m_left);
        cout << node->m_data << " ";
        printInOrder(node->m_right);
    }

    Node* getMinNode(Node* node) const {
        while (node && node->m_left) {
            node = node->m_left;
        }
        return node;
    }

    void clear(Node* node) {
        if (!node) return;

        clear(node->m_left);
        clear(node->m_right);
        delete node;
    }
};
 
