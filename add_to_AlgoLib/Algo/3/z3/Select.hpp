#ifndef SELECT_HPP
#define SELECT_HPP

#include <vector>
#include <cstddef>
#include <iostream>

/// Counts how many comparisons and swaps occur.
struct Counter {
    size_t comparisons = 0;
    size_t swaps = 0;
};

/// Deterministic “Select” (median‐of‐medians) interface:
/// - A: the array of integers
/// - left, right: inclusive subarray indices
/// - k: 0-based order statistic (k=0 → smallest, etc.)
/// - cnt: increments for each comparison/swap
/// - verbose: if true, prints array after every swap/partition
///
/// Returns the k-th smallest value in A[left..right].
int Select(std::vector<int>& A, size_t left, size_t right, size_t k, Counter& cnt, bool verbose);

/// Partition A[left..right] around A[pivotIndex].
/// Returns the final index of the pivot. Increments cnt for each comparison/swap.
size_t Partition(std::vector<int>& A, size_t left, size_t right, size_t pivotIndex, Counter& cnt, bool verbose);

/// Finds the “median of medians” index in A[left..right], using groups of 5.
/// Returns a size_t index to be used as pivot.
size_t MedianOfMedians(std::vector<int>& A, size_t left, size_t right, Counter& cnt, bool verbose);

#endif // SELECT_HPP
