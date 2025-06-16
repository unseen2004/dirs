#ifndef RANDOMIZED_SELECT_HPP
#define RANDOMIZED_SELECT_HPP

#include <vector>
#include <cstddef>
#include "Select.hpp"

/// RandomizedSelect: exactly like Quickselect, but each time pick a random pivot.
int RandomizedSelect(std::vector<int>& A, size_t left, size_t right, size_t k, Counter& cnt, bool verbose);

/// Pick a random pivot, move it to A[right], then standard partition.
size_t RandomizedPartition(std::vector<int>& A, size_t left, size_t right, Counter& cnt, bool verbose);

#endif // RANDOMIZED_SELECT_HPP
