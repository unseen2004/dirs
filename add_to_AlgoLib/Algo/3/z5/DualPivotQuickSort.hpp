#ifndef DUALPIVOTQUICKSORT_HPP
#define DUALPIVOTQUICKSORT_HPP

#include <vector>
#include "Select.hpp"

/// Randomized Dual-Pivot Quicksort: pick two random pivots, partition into three blocks.
void DualPivotRandom(std::vector<int>& A,
                     int left,
                     int right,
                     Counter& cnt,
                     bool verbose);

/// SELECT‐Pivot Dual-Pivot Quicksort: use SELECT to find the 1/3 and 2/3 order stats as pivots.
void DualPivotSelect(std::vector<int>& A,
                     int left,
                     int right,
                     Counter& cnt,
                     bool verbose);

#endif // DUALPIVOTQUICKSORT_HPP

