#include "SelectVariant.hpp"
#include <algorithm>

// Helper for verbose printing.
static void printStateVariant(const std::vector<int>& A) {
    for (size_t i = 0; i < A.size(); ++i) {
        std::cout << A[i] << (i + 1 < A.size() ? ' ' : '\n');
    }
}

size_t PartitionVariant(std::vector<int>& A, size_t left, size_t right, size_t pivotIndex, Counter& cnt, bool verbose) {
    int pivotValue = A[pivotIndex];
    std::swap(A[pivotIndex], A[right]);
    cnt.swaps++;
    if (verbose) printStateVariant(A);

    size_t storeIndex = left;
    for (size_t i = left; i < right; ++i) {
        cnt.comparisons++;
        if (A[i] < pivotValue) {
            std::swap(A[storeIndex], A[i]);
            cnt.swaps++;
            storeIndex++;
            if (verbose) printStateVariant(A);
        }
    }
    std::swap(A[right], A[storeIndex]);
    cnt.swaps++;
    if (verbose) printStateVariant(A);
    return storeIndex;
}

size_t MedianOfMediansVariant(std::vector<int>& A, size_t left, size_t right, Counter& cnt, bool verbose, size_t groupSize) {
    if (right < left) return left;
    if (right - left < groupSize) {
        // If fewer than groupSize elements, just sort that subarray and return its middle index.
        std::sort(A.begin() + left, A.begin() + right + 1);
        return left + (right - left) / 2;
    }

    // Otherwise, break into blocks of exactly `groupSize` (last block may be smaller).
    size_t numMedians = 0;
    for (size_t i = left; i <= right; i += groupSize) {
        size_t subRight = std::min(right, i + groupSize - 1);
        std::sort(A.begin() + i, A.begin() + subRight + 1);
        size_t medianIndex = i + (subRight - i) / 2;
        std::swap(A[left + numMedians], A[medianIndex]);
        cnt.swaps++;
        if (verbose) printStateVariant(A);
        numMedians++;
    }

    // Now A[left..left+numMedians-1] holds the medians of each block.
    // Recursively find the median-of-medians value in that region:
    int medianValue = SelectVariant(A, left, left + numMedians - 1, left + numMedians / 2, cnt, verbose, groupSize);

    // Find the index of that medianValue among A[left..left+numMedians-1]
    for (size_t i = left; i < left + numMedians; ++i) {
        if (A[i] == medianValue) {
            return i;
        }
    }
    return left + numMedians / 2; // fallback, should not be needed
}

int SelectVariant(std::vector<int>& A, size_t left, size_t right, size_t k, Counter& cnt, bool verbose, size_t groupSize) {
    if (left == right) return A[left];
    size_t pivotIndex = MedianOfMediansVariant(A, left, right, cnt, verbose, groupSize);
    size_t pivotNewIndex = PartitionVariant(A, left, right, pivotIndex, cnt, verbose);
    if (k == pivotNewIndex) {
        return A[k];
    } else if (k < pivotNewIndex) {
        return SelectVariant(A, left, pivotNewIndex - 1, k, cnt, verbose, groupSize);
    } else {
        return SelectVariant(A, pivotNewIndex + 1, right, k, cnt, verbose, groupSize);
    }
}
