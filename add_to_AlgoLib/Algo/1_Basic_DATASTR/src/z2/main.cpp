#include <iostream>
#include <vector>
#include "CyclicList.hpp"
#include "Random.hpp"

int main() {
    CyclicList<int> list1{};
    CyclicList<int> list2{};

    for (int i = 0; i < 10; i++) {
        int num = Random::get(10, 99);
        list1.insert(num);
    }

    std::cout << "List1 after insertion: ";
    list1.print();

    for (int i = 0; i < 10; i++) {
        int num = Random::get(10, 99);
        list2.insert(num);
    }
    std::cout << "List2 after insertion: ";
    list2.print();

    std::cout << "Merging List2 into List1\n";
    list1.merge(list2);
    list1.print();

    const int T_SIZE = 10000;
    const int SEARCH_COUNT = 1000;
    const int RANGE_MAX = 100000;

    std::vector<int> T{};
    T.reserve(T_SIZE);
    for (int i = 0; i < T_SIZE; i++) {
        T.push_back(Random::get(0, RANGE_MAX));
    }

    CyclicList<int> L{};
    for (auto val : T) {
        L.insert(val);
    }

    long long totalComparisonsFound = 0;
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int randomIndex = Random::get(0, T_SIZE - 1);
        int target = T[randomIndex];
        try {
            totalComparisonsFound += L.search(target);
        } catch (const std::runtime_error& e) {}
    }
    double avgComparisonsFound = static_cast<double>(totalComparisonsFound) / SEARCH_COUNT;
    std::cout << avgComparisonsFound << "\n";

    long long totalComparisonsRandom = 0;
    for (int i = 0; i < SEARCH_COUNT; i++) {
        int target = Random::get(0, RANGE_MAX);
        try {
            totalComparisonsRandom += L.search(target);
        } catch (const std::runtime_error& e) {
            totalComparisonsRandom += L.size;
        }
    }
    double avgComparisonsRandom = static_cast<double>(totalComparisonsRandom) / SEARCH_COUNT;
    std::cout << avgComparisonsRandom << "\n";

    return 0;
}