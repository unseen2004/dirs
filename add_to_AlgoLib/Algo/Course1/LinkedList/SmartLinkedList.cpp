#include <memory>

template <typename T>
class List {
    struct Node {
        T data;
        std::unique_ptr<Node> next;

        Node(T n) : data(n), next(nullptr) {}
    };

    std::unique_ptr<Node> head;

public:
    List() : head(nullptr) {}

    ~List() {
        while (head) {
            pop();
        }
    }

    void add(T n) {
        auto newNode = std::make_unique<Node>(n);
        if (head) {
            Node* tmp = head.get();
            while (tmp->next) {
                tmp = tmp->next.get();
            }
            tmp->next = std::move(newNode);
        } else {
            head = std::move(newNode);
        }
    }

    void pop() {
        if (!head) {
            return;
        } else if (!head->next) {
            head.reset();
            return;
        } else {
            Node* tmp = head.get();
            while (tmp->next && tmp->next->next) {
                tmp = tmp->next.get();
            }
            tmp->next.reset(); // Deletes the last node
        }
    }

};
