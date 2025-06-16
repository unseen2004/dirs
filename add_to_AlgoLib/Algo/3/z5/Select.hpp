#ifndef SELECT_HPP
#define SELECT_HPP

#include <vector>
#include <cstddef>
#include <iostream>

/// A simple Counter that counts only comparisons.
struct Counter {
    size_t comparisons = 0;
};

/// Deterministic SELECT (median‐of‐medians) interface:
/// - A: the array of ints
/// - left, right: inclusive subarray bounds
/// - k: 0‐based order statistic (k=0 means smallest)
/// - cnt: increment cnt.comparisons whenever comparing array elements
/// - verbose: if true, print intermediate states
///
/// Returns the k-th smallest value (not index).
int Select(std::vector<int>& A,
           size_t left,
           size_t right,
           size_t k,
           Counter& cnt,
           bool verbose);

/// Partition A[left..right] around A[pivotIndex], counting comparisons.
/// Returns the final index of the pivot.
size_t PartitionSelect(std::vector<int>& A,
                       size_t left,
                       size_t right,
                       size_t pivotIndex,
                       Counter& cnt,
                       bool verbose);

/// Find the “median of medians” pivot index in A[left..right].
/// Uses groups of 5. Returns an index in [left..right].
size_t MedianOfMedians(std::vector<int>& A,
                       size_t left,
                       size_t right,
                       Counter& cnt,
                       bool verbose);

#endif // SELECT_HPP
