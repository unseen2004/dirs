#include "DualPivotQuickSort.hpp"
#include "Random.hpp"
#include <algorithm>

// Print the entire array on one line (verbose)
static void printDPQS(const std::vector<int>& A) {
    for (size_t i = 0; i < A.size(); ++i)
        std::cout << A[i] << (i + 1 < A.size() ? ' ' : '\n');
}

// Dual-pivot partition routine: expects pivots at A[left] ≤ A[right].
// After rearranging, it sets lp=final index of pivot1, rp=final index of pivot2.
static void DualPivotPartition(std::vector<int>& A,
                               int left,
                               int right,
                               int& lp,
                               int& rp,
                               Counter& cnt,
                               bool verbose)
{
    if (A[left] > A[right]) {
        std::swap(A[left], A[right]);
        if (verbose) printDPQS(A);
    }
    int p = A[left];
    int q = A[right];
    int i = left + 1;
    int lt = left + 1;
    int gt = right - 1;

    while (i <= gt) {
        cnt.comparisons++;
        if (A[i] < p) {
            std::swap(A[i], A[lt]);
            lt++;
            i++;
            if (verbose) printDPQS(A);
        }
        else if (A[i] > q) {
            std::swap(A[i], A[gt]);
            gt--;
            if (verbose) printDPQS(A);
        }
        else {
            i++;
        }
    }
    lt--;
    gt++;
    std::swap(A[left], A[lt]);
    std::swap(A[right], A[gt]);
    if (verbose) printDPQS(A);
    lp = lt;
    rp = gt;
}

void DualPivotRandom(std::vector<int>& A,
                     int left,
                     int right,
                     Counter& cnt,
                     bool verbose)
{
    if (left >= right) return;
    int idx1 = Random::get<int>(left, right);
    int idx2 = Random::get<int>(left, right);
    std::swap(A[left], A[idx1]);
    std::swap(A[right], A[idx2]);
    if (verbose) printDPQS(A);

    int lp, rp;
    DualPivotPartition(A, left, right, lp, rp, cnt, verbose);
    DualPivotRandom(A, left, lp - 1, cnt, verbose);
    DualPivotRandom(A, lp + 1, rp - 1, cnt, verbose);
    DualPivotRandom(A, rp + 1, right, cnt, verbose);
}

void DualPivotSelect(std::vector<int>& A,
                     int left,
                     int right,
                     Counter& cnt,
                     bool verbose)
{
    if (left >= right) return;
    int size = right - left + 1;

    // Make a local copy for SELECT to find the 1/3 and 2/3 values
    std::vector<int> temp(A.begin() + left, A.begin() + right + 1);
    Counter cntSelect;
    int val1 = Select(temp, 0, (int)temp.size() - 1, size / 3, cntSelect, false);
    int val2 = Select(temp, 0, (int)temp.size() - 1, 2 * size / 3, cntSelect, false);

    // Find their positions in A[left..right]
    int idx1 = left, idx2 = right;
    bool found1 = false, found2 = false;
    for (int i = left; i <= right; ++i) {
        cnt.comparisons++;
        if (!found1 && A[i] == val1) {
            idx1 = i;
            found1 = true;
        }
        else if (!found2 && A[i] == val2) {
            idx2 = i;
            found2 = true;
        }
    }
    std::swap(A[left], A[idx1]);
    std::swap(A[right], A[idx2]);
    if (verbose) printDPQS(A);

    int lp, rp;
    DualPivotPartition(A, left, right, lp, rp, cnt, verbose);
    DualPivotSelect(A, left, lp - 1, cnt, verbose);
    DualPivotSelect(A, lp + 1, rp - 1, cnt, verbose);
    DualPivotSelect(A, rp + 1, right, cnt, verbose);
}
