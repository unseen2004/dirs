
template<typename T>
struct Node {
    T value;
    Node* next;
    Node* prev;

    Node(T n) : value(n), next(nullptr), prev(nullptr) {}
};

template<typename T>
class DoubleLinkedList {
    Node<T>* head;
    Node<T>* tail;
    int len;

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), len(0) {}

    ~DoubleLinkedList() {
        while (head) {
            removeBack();
        }
    }

    int getLen() {
        return len;
    }

    void prepend(T data) {
        Node<T>* newNode = new Node<T>(data);
        ++len;
        if (head) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        } else {
            head = tail = newNode;
        }
    }

    void append(T data) {
        Node<T>* newNode = new Node<T>(data);
        ++len;
        if (tail) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else {
            head = tail = newNode;
        }
    }

    bool insertAt(Node<T>* prevNode, T data) {
        if (!prevNode) return false;

        Node<T>* newNode = new Node<T>(data);
        ++len;
        if (!prevNode->next) {
            prevNode->next = newNode;
            newNode->prev = prevNode;
            tail = newNode;
        } else {
            newNode->next = prevNode->next;
            prevNode->next->prev = newNode;
            newNode->prev = prevNode;
            prevNode->next = newNode;
        }
        return true;
    }

    bool removeFront() {
        if (head) {
            --len;
            if (head->next) {
                Node<T>* tmp = head;
                head = head->next;
                head->prev = nullptr;
                delete tmp;
            } else {
                delete head;
                head = tail = nullptr;
            }
            return true;
        }
        return false;
    }

    bool removeBack() {
        if (tail) {
            --len;
            if (tail->prev) {
                Node<T>* tmp = tail;
                tail = tail->prev;
                tail->next = nullptr;
                delete tmp;
            } else {
                delete tail;
                head = tail = nullptr;
            }
            return true;
        }
        return false;
    }

    bool removeAt(Node<T>* node) {
        if (node) {
            --len;
            if (!node->prev) {
                return removeFront();
            } else if (!node->next) {
                return removeBack();
            } else {
                node->next->prev = node->prev;
                node->prev->next = node->next;
                delete node;
                return true;
            }
        }
        return false;
    }
};

