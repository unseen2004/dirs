#include <iostream>
#include "../../includes/Queue.hpp"
#include "../../includes/Stack.hpp"

int main() {
    Queue<int> fifo;
    Stack<int> lifo;

    std::cout << "Enqueuing into Queue:" << std::endl;
    for (int i = 1; i <= 50; ++i) {
        fifo.enqueue(i);
        std::cout << "Enqueued: " << i << std::endl;
    }

    std::cout << "\nDequeuing from Queue:" << std::endl;
    try {
        while (!fifo.is_empty()) {
            int value = fifo.peek();
            std::cout << "Peek: " << value << std::endl;
            fifo.dequeue();
        }
        fifo.dequeue();
    } catch (const std::exception &e) {
        std::cout << "Queue error: " << e.what() << std::endl;
    }

    std::cout << "\nPushing onto Stack:" << std::endl;
    for (int i = 1; i <= 50; ++i) {
        lifo.push(i);
        std::cout << "Pushed: " << i << std::endl;
    }

    std::cout << "\nPopping from Stack:" << std::endl;
    try {
        while (!lifo.is_empty()) {
            int value = lifo.peek();
            std::cout << "Peek: " << value << std::endl;
            lifo.pop();
        }
        lifo.pop();
    } catch (const std::exception &e) {
        std::cout << "Stack error: " << e.what() << std::endl;
    }

}