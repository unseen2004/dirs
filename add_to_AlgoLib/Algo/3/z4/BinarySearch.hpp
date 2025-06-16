#ifndef BINARYSEARCH_HPP
#define BINARYSEARCH_HPP

#include <vector>
#include <cstddef>
#include <iostream>

/// Counter for number of element comparisons.
struct Counter {
    size_t comparisons = 0;
};

/// Recursive binary search on sorted array A[left..right].
/// Returns true if value v is found; false otherwise.
/// Increments cnt.comparisons for each comparison of A[mid] to v.
/// If verbose=true, prints each comparison and recursive branch.
bool binarySearch(const std::vector<int>& A, int left, int right, int v, Counter& cnt, bool verbose);

#endif // BINARYSEARCH_HPP
