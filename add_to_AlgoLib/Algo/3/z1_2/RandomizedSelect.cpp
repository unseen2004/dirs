#include "RandomizedSelect.hpp"
#include "Random.hpp"
#include <algorithm>
#include <iostream>

static void printState(const std::vector<int>& A) {
    for (size_t i = 0; i < A.size(); ++i) {
        std::cout << A[i] << (i + 1 < A.size() ? ' ' : '\n');
    }
}

size_t RandomizedPartition(std::vector<int>& A, size_t left, size_t right, Counter& cnt, bool verbose) {
    size_t pivotIndex = Random::get<size_t>(left, right);
    std::swap(A[pivotIndex], A[right]);
    cnt.swaps++;
    if (verbose) printState(A);

    int pivotValue = A[right];
    size_t storeIndex = left;
    for (size_t i = left; i < right; ++i) {
        cnt.comparisons++;
        if (A[i] < pivotValue) {
            std::swap(A[storeIndex], A[i]);
            cnt.swaps++;
            storeIndex++;
            if (verbose) printState(A);
        }
    }
    std::swap(A[right], A[storeIndex]);
    cnt.swaps++;
    if (verbose) printState(A);
    return storeIndex;
}

int RandomizedSelect(std::vector<int>& A, size_t left, size_t right, size_t k, Counter& cnt, bool verbose) {
    if (left == right) {
        return A[left];
    }
    size_t pivotNewIndex = RandomizedPartition(A, left, right, cnt, verbose);
    if (k == pivotNewIndex) {
        return A[k];
    } else if (k < pivotNewIndex) {
        return RandomizedSelect(A, left, pivotNewIndex - 1, k, cnt, verbose);
    } else {
        return RandomizedSelect(A, pivotNewIndex + 1, right, k, cnt, verbose);
    }
}
