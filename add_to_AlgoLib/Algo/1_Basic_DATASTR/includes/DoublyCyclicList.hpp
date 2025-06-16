#ifndef DOUBLYCYCLICLIST_HPP
#define DOUBLYCYCLICLIST_HPP

#include "Node.hpp"
#include <stdexcept>
#include <cstdlib>
#include <iostream>

template<typename T>
class DoublyCyclicList {
public:
    Node<T>* tail;
    int size;
    DoublyCyclicList() : tail{nullptr}, size{0} {}

    auto insert(const T& val) -> void {
        auto* newNode = new Node<T>{val};
        if (!tail) {
            newNode->left = newNode;
            newNode->right = newNode;
            tail = newNode;
        } else {
            Node<T>* head = tail->right;
            newNode->left = tail;
            newNode->right = head;
            tail->right = newNode;
            head->left = newNode;
            tail = newNode;
        }
        size++;
    }

    auto search(const T& val) -> int {
        if (!tail) throw std::runtime_error("List is empty");
        int comparisons = 0;
        Node<T>* current;
        Node<T>* start;
        if (std::rand() % 2 == 0) {
            current = tail->right;
            start = current;
            do {
                comparisons++;
                if (current->val == val) return comparisons;
                current = current->right;
            } while (current != start);
        } else {
            current = tail;
            start = current;
            do {
                comparisons++;
                if (current->val == val) return comparisons;
                current = current->left;
            } while (current != start);
        }
        throw std::runtime_error("Element not found");
    }

    auto print() const -> void {
        if (!tail) {
            std::cout << "List is empty\n";
            return;
        }
        Node<T>* current = tail->right;
        Node<T>* start = current;
        do {
            std::cout << current->val << " ";
            current = current->right;
        } while (current != start);
        std::cout << "\n";
    }

    auto merge(DoublyCyclicList<T>& other) -> void {
        if (!other.tail) return;
        if (!tail) {
            tail = other.tail;
            size = other.size;
            other.tail = nullptr;
            other.size = 0;
            return;
        }
        Node<T>* head1 = tail->right;
        Node<T>* head2 = other.tail->right;
        tail->right = head2;
        head2->left = tail;
        other.tail->right = head1;
        head1->left = other.tail;
        tail = other.tail;
        size += other.size;
        other.tail = nullptr;
        other.size = 0;
    }

    ~DoublyCyclicList() {
        if (tail) {
            Node<T>* head = tail->right;
            tail->right = nullptr;
            while (head) {
                Node<T>* temp = head;
                head = head->right;
                delete temp;
            }
        }
    }
};

#endif
