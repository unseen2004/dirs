#ifndef CYCLICLIST_HPP
#define CYCLICLIST_HPP

#include "Node.hpp"
#include <stdexcept>
#include <iostream>

template<typename T>
class CyclicList {
public:
    node<T>* tail;
    int size;
    CyclicList() : tail{nullptr}, size{0} {}

    auto insert(const T& val) -> void {
        auto* newNode = new node<T>{val};
        if (!tail) {
            newNode->next = newNode;
            tail = newNode;
        } else {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    auto search(const T& val) {
        if (!tail) throw std::runtime_error("List is empty");
        int comparisons = 0;
        node<T>* current = tail->next;
        do {
            comparisons++;
            if (current->val == val) return comparisons;
            current = current->next;
        } while (current != tail->next);
        throw std::runtime_error("Element not found");
    }

    auto print() const -> void {
        if (!tail) {
            std::cout << "List is empty\n";
            return;
        }
        node<T>* current = tail->next;
        do {
            std::cout << current->val << " ";
            current = current->next;
        } while (current != tail->next);
        std::cout << "\n";
    }

    auto merge(CyclicList<T>& other) -> void {
        if (!other.tail) return;
        if (!tail) {
            tail = other.tail;
            size = other.size;
            other.tail = nullptr;
            other.size = 0;
            return;
        }
        node<T>* head1 = tail->next;
        node<T>* head2 = other.tail->next;
        tail->next = head2;
        other.tail->next = head1;
        tail = other.tail;
        size += other.size;
        other.tail = nullptr;
        other.size = 0;
    }

    ~CyclicList() {
        if (tail) {
            node<T>* head = tail->next;
            tail->next = nullptr;
            while (head) {
                auto* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }
};

#endif
