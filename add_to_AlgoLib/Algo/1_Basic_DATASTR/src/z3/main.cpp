#include <iostream>
#include <vector>
#include "DoublyCyclicList.hpp"
#include "Random.hpp"

int main() {
    DoublyCyclicList<int> list1{};
    DoublyCyclicList<int> list2{};

    std::cout << "Inserting into list1:\n";
    for (int i = 0; i < 10; i++) {
        int num = Random::get(10, 99);
        list1.insert(num);
        std::cout << "Inserted: " << num << "\n";
    }
    list1.print();

    std::cout << "Inserting into list2:\n";
    for (int i = 0; i < 10; i++) {
        int num = Random::get(10, 99);
        list2.insert(num);
        std::cout << "Inserted: " << num << "\n";
    }
    list2.print();

    std::cout << "Merging list2 into list1\n";
    list1.merge(list2);
    list1.print();

    const int T_SIZE = 10000;
    const int SEARCH_COUNT = 1000;
    const int RANGE_MAX = 100000;

    std::vector<int> T{};
    T.reserve(T_SIZE);
    std::cout << "Generating random numbers for T:\n";
    for (int i = 0; i < T_SIZE; i++) {
        int num = Random::get(0, RANGE_MAX);
        T.push_back(num);
    }

    DoublyCyclicList<int> L{};
    std::cout << "Inserting into L:\n";
    for (auto val : T) {
        L.insert(val);
    }

    long long totalComparisonsFound = 0;
    std::cout << "Searching for elements in T:\n";
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int randomIndex = Random::get(0, T_SIZE - 1);
        int target = T[randomIndex];
        try {
            totalComparisonsFound += L.search(target);
        } catch (const std::runtime_error&) {}
    }
    double avgComparisonsFound = static_cast<double>(totalComparisonsFound) / SEARCH_COUNT;
    std::cout << "Average comparisons for found elements: " << avgComparisonsFound << "\n";

    long long totalComparisonsRandom = 0;
    std::cout << "Searching for random elements:\n";
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int target = Random::get(0, RANGE_MAX);
        try {
            totalComparisonsRandom += L.search(target);
        } catch (const std::runtime_error&) {
            totalComparisonsRandom += L.size;
        }
    }
    double avgComparisonsRandom = static_cast<double>(totalComparisonsRandom) / SEARCH_COUNT;
    std::cout << "Average comparisons for random elements: " << avgComparisonsRandom << "\n";

    return 0;
}