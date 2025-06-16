#include "QuickSort.hpp"
#include "Random.hpp"
#include <algorithm>

// Prints the entire array on one line (for verbose)
static void printQS(const std::vector<int>& A) {
    for (size_t i = 0; i < A.size(); ++i)
        std::cout << A[i] << (i + 1 < A.size() ? ' ' : '\n');
}

// Standard partition: pivot is A[right]
static int PartitionQS(std::vector<int>& A,
                       int left,
                       int right,
                       Counter& cnt,
                       bool verbose)
{
    int pivotValue = A[right];
    int i = left;
    for (int j = left; j < right; ++j) {
        cnt.comparisons++;
        if (A[j] < pivotValue) {
            std::swap(A[i], A[j]);
            i++;
            if (verbose) printQS(A);
        }
    }
    std::swap(A[i], A[right]);
    if (verbose) printQS(A);
    return i;
}

void QuickSortRandom(std::vector<int>& A,
                     int left,
                     int right,
                     Counter& cnt,
                     bool verbose)
{
    if (left >= right) return;
    int pivotIndex = Random::get<int>(left, right);
    std::swap(A[pivotIndex], A[right]);
    if (verbose) printQS(A);
    int p = PartitionQS(A, left, right, cnt, verbose);
    QuickSortRandom(A, left, p - 1, cnt, verbose);
    QuickSortRandom(A, p + 1, right, cnt, verbose);
}

void QuickSortSelectPivot(std::vector<int>& A,
                          int left,
                          int right,
                          Counter& cnt,
                          bool verbose)
{
    if (left >= right) return;
    // Make a local copy of A[left..right], run SELECT on it to find its median value
    std::vector<int> temp(A.begin() + left, A.begin() + right + 1);
    Counter cntSelect;
    int medianVal = Select(temp, 0, (int)temp.size() - 1, temp.size() / 2, cntSelect, false);

    // Find the index of medianVal in the original A[left..right]
    int pivotIndex = left;
    for (int i = left; i <= right; ++i) {
        cnt.comparisons++;
        if (A[i] == medianVal) {
            pivotIndex = i;
            break;
        }
    }
    std::swap(A[pivotIndex], A[right]);
    if (verbose) printQS(A);
    int p = PartitionQS(A, left, right, cnt, verbose);
    QuickSortSelectPivot(A, left, p - 1, cnt, verbose);
    QuickSortSelectPivot(A, p + 1, right, cnt, verbose);
}
