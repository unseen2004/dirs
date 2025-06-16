#ifndef SELECT_HPP
#define SELECT_HPP

#include <vector>
#include <cstddef>
#include <iostream>

/// Count how many comparisons and swaps occur.
struct Counter {
    size_t comparisons = 0;
    size_t swaps = 0;
};

/// Deterministic “Select” (median‐of‐medians) interface.
/// - A: array of ints
/// - left, right: inclusive subarray bounds
/// - k: 0-based order statistic (i.e. k=0 means find the minimum)
/// - cnt: counts comparisons/swaps
/// - verbose: if true, print array after every swap/partition
///
/// Returns the k-th smallest *value* in A[left..right].
int Select(std::vector<int>& A, size_t left, size_t right, size_t k, Counter& cnt, bool verbose);

/// Partition A[left..right] around A[pivotIndex].
/// Returns the final *index* of that pivot. Increments cnt accordingly.
size_t Partition(std::vector<int>& A, size_t left, size_t right, size_t pivotIndex, Counter& cnt, bool verbose);

/// Find the “median of medians” index in A[left..right].
/// Returns a size_t index that is guaranteed to be a good pivot.
size_t MedianOfMedians(std::vector<int>& A, size_t left, size_t right, Counter& cnt, bool verbose);

#endif // SELECT_HPP
