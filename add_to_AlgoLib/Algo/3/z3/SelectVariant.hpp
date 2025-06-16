#ifndef SELECT_VARIANT_HPP
#define SELECT_VARIANT_HPP

#include <vector>
#include <cstddef>
#include <iostream>
#include "Select.hpp"  // for Counter

/// Same as Partition(...) in Select, but factored out for variants.
size_t PartitionVariant(std::vector<int>& A, size_t left, size_t right, size_t pivotIndex, Counter& cnt, bool verbose);

/// “Median of medians” but using `groupSize` instead of 5.
/// Groups A[left..right] into blocks of size = groupSize (last block may be smaller),
/// sorts each block to pick its median, moves those medians to A[left..left+numMedians-1],
/// then recurses to find the “median of medians.”
/// Returns an index in [left..right] that we will use as a pivot.
size_t MedianOfMediansVariant(std::vector<int>& A, size_t left, size_t right, Counter& cnt, bool verbose, size_t groupSize);

/// Generalized Select that uses MedianOfMediansVariant(..., groupSize).
/// - A[left..right]: subarray
/// - k: 0-based order statistic in that subarray
/// - groupSize: 3, 5, 7, or 9
int SelectVariant(std::vector<int>& A, size_t left, size_t right, size_t k, Counter& cnt, bool verbose, size_t groupSize);

#endif // SELECT_VARIANT_HPP
