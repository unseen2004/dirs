

#include <optional>

template<typename T>
class Stack {
    struct Node {
        T value;
        Node* prev;

        Node(T data) : value(data), prev(nullptr) {}
    };

    Node* m_head;
    int len;

public:
    Stack() : m_head(nullptr), len(0) {}

    ~Stack() {
        while (m_head) {
            pop();
        }
    }

    void pop() {
        if (m_head) {
            Node* tmp = m_head;
            m_head = m_head->prev;
            delete tmp;
            --len; 
        }
    }

    void push(T data) {
        Node* newNode = new Node(data);
        newNode->prev = m_head; 
        m_head = newNode; 
        ++len; 
    }

    std::optional<T> peak() {
        if (!m_head) {
            return {}; 
        }
        return m_head->value; 
    }

    int size() const {
        return len; 
    }

    bool isEmpty() const {
        return len == 0; 
    }
};

