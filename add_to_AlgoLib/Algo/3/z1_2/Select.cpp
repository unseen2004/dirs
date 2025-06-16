#include "Select.hpp"
#include <algorithm>

// Helper: print the entire array (one line) when verbose = true.
static void printState(const std::vector<int>& A) {
    for (size_t i = 0; i < A.size(); ++i) {
        std::cout << A[i] << (i + 1 < A.size() ? ' ' : '\n');
    }
}

size_t Partition(std::vector<int>& A, size_t left, size_t right, size_t pivotIndex, Counter& cnt, bool verbose) {
    int pivotValue = A[pivotIndex];
    std::swap(A[pivotIndex], A[right]);
    cnt.swaps++;
    if (verbose) printState(A);

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

size_t MedianOfMedians(std::vector<int>& A, size_t left, size_t right, Counter& cnt, bool verbose) {
    // If fewer than 5 elements, sort and pick the middle index.
    if (right - left < 5) {
        std::sort(A.begin() + left, A.begin() + right + 1);
        return left + (right - left) / 2;
    }

    // Partition into groups of up to 5 each, sort each group,
    // then move each group's median to A[left + numMedians].
    size_t numMedians = 0;
    for (size_t i = left; i <= right; i += 5) {
        size_t subRight = std::min(right, i + 4);
        std::sort(A.begin() + i, A.begin() + subRight + 1);
        size_t medianIndex = i + (subRight - i) / 2;
        std::swap(A[left + numMedians], A[medianIndex]);
        cnt.swaps++;
        if (verbose) printState(A);
        ++numMedians;
    }

    // Now the medians of each group live in A[left..left+numMedians-1].
    // Recursively select the median-value of those medians:
    int medianValue = Select(A, left, left + numMedians - 1, left + numMedians / 2, cnt, verbose);

    // Find where that medianValue sits among A[left..left+numMedians-1].
    for (size_t i = left; i < left + numMedians; ++i) {
        if (A[i] == medianValue) {
            return i;
        }
    }
    // (Should never happen; fallback):
    return left + numMedians / 2;
}

int Select(std::vector<int>& A, size_t left, size_t right, size_t k, Counter& cnt, bool verbose) {
    if (left == right) {
        return A[left];
    }
    // 1. Choose pivot by median‐of‐medians:
    size_t pivotIndex = MedianOfMedians(A, left, right, cnt, verbose);

    // 2. Partition around pivotIndex:
    size_t pivotNewIndex = Partition(A, left, right, pivotIndex, cnt, verbose);

    // 3. Recurse into the correct side:
    if (k == pivotNewIndex) {
        return A[k];
    } else if (k < pivotNewIndex) {
        return Select(A, left, pivotNewIndex - 1, k, cnt, verbose);
    } else {
        return Select(A, pivotNewIndex + 1, right, k, cnt, verbose);
    }
}
