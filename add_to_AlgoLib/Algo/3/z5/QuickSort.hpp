#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <vector>
#include "Select.hpp"

/// Randomized Quicksort: pick a random pivot each call, partition, recurse.
void QuickSortRandom(std::vector<int>& A,
                     int left,
                     int right,
                     Counter& cnt,
                     bool verbose);

/// SELECT‐Pivot Quicksort: at each call, use SELECT to pick the median as pivot.
void QuickSortSelectPivot(std::vector<int>& A,
                          int left,
                          int right,
                          Counter& cnt,
                          bool verbose);

#endif // QUICKSORT_HPP

