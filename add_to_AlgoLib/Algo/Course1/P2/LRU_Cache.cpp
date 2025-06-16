
#include <iostream>
#include <unordered_map>
#include <stdexcept>

template <typename T>
struct Node {
    T value;
    Node* next;
    Node* prev;

    Node(T val) : value(val), next(nullptr), prev(nullptr) {}
};

template <typename K, typename V>
class LRU_Cache {
    using NodePtr = Node<V>*;

    int m_length;
    NodePtr m_head;
    NodePtr m_tail;
    const int m_capacity;
    std::unordered_map<K, NodePtr> look_up;
    std::unordered_map<NodePtr, K> reverse_look_up;

public:
    LRU_Cache(int capacity = 10) 
        : m_capacity(capacity), m_length(0), m_head(nullptr), m_tail(nullptr) {}

    ~LRU_Cache() {
        // Cleanup all allocated nodes
        NodePtr current = m_head;
        while (current) {
            NodePtr to_delete = current;
            current = current->next;
            delete to_delete;
        }
    }

    void update(K key, V value) {
        // Check if the key exists
        if (look_up.find(key) != look_up.end()) {
            NodePtr node = look_up[key];
            node->value = value; // Update value
            detach(node);
            prepend(node);
        } else {
            // Insert a new node
            if (m_length == m_capacity) {
                trimCache();
            }
            NodePtr node = new Node<V>(value);
            prepend(node);
            look_up[key] = node;
            reverse_look_up[node] = key;
            ++m_length;
        }
    }

    V get(K key) {
        if (look_up.find(key) != look_up.end()) {
            NodePtr node = look_up[key];
            detach(node);
            prepend(node);
            return node->value;
        }
        throw std::out_of_range("Key not found");
    }

private:
    void detach(NodePtr node) {
        if (node->next) {
            node->next->prev = node->prev;
        }
        if (node->prev) {
            node->prev->next = node->next;
        }
        if (node == m_head) {
            m_head = node->next;
        }
        if (node == m_tail) {
            m_tail = node->prev;
        }
        node->next = nullptr;
        node->prev = nullptr;
    }

    void prepend(NodePtr node) {
        if (!m_head) {
            m_head = m_tail = node;
            return;
        }
        node->next = m_head;
        m_head->prev = node;
        m_head = node;
    }

    void trimCache() {
        if (!m_tail) return;

        NodePtr node_to_remove = m_tail;
        K key_to_remove = reverse_look_up[node_to_remove];

        detach(node_to_remove);

        look_up.erase(key_to_remove);
        reverse_look_up.erase(node_to_remove);

        delete node_to_remove;
        --m_length;
    }
};

