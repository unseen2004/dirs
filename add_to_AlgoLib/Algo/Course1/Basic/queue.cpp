
template<typename T>
class Queue {
    struct Node {
        T value;
        Node* next;

        Node(T data) : value(data), next(nullptr) {}
    };

    Node* m_head;
    Node* m_tail;

public:
    Queue() : m_head(nullptr), m_tail(nullptr) {}

    ~Queue() {
        while (m_head) {
            dequeue();
        }
    }

    void enqueue(T data) {
        Node* newNode = new Node(data);
        if (m_tail) {
            m_tail->next = newNode;
            m_tail = newNode;
        } else {
            m_head = m_tail = newNode;
        }
    }

    void dequeue() {
        if (m_head) {
            Node* temp = m_head;
            m_head = m_head->next;
            delete temp;
            if (!m_head) {
                m_tail = nullptr;
            }
        }
    }
};

