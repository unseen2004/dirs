#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include "QuickSort.hpp"
#include "DualPivotQuickSort.hpp"
#include "Random.hpp"

/// Utility to fill an array of size n with random ints in [0..2n-1].
void generateData(std::vector<int>& A) {
    int n = (int)A.size();
    for (int i = 0; i < n; ++i) {
        A[i] = Random::get<int>(0, 2 * n - 1);
    }
}

/// Run one batch of experiments for Quicksort variants (Random vs SelectPivot),
/// each repeated m times on fresh random arrays of length n.
/// Append lines to “quick_results.csv” with columns:
///    n,algorithm,comparisons,time
void runQuickOnce(int n, int m) {
    std::ofstream ofs("quick_results.csv", std::ios::app);
    // Two algorithms: 0=QuickRandom, 1=QuickSelect
    for (int alg = 0; alg < 2; ++alg) {
        double totalTime = 0.0;
        double totalComp = 0.0;
        for (int iter = 0; iter < m; ++iter) {
            std::vector<int> A(n);
            generateData(A);
            Counter cnt;
            auto start = std::chrono::high_resolution_clock::now();
            if (alg == 0) {
                QuickSortRandom(A, 0, n - 1, cnt, false);
            } else {
                QuickSortSelectPivot(A, 0, n - 1, cnt, false);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            totalTime += diff.count();
            totalComp += cnt.comparisons;
        }
        double avgTime = totalTime / m;
        double avgComp = totalComp / m;
        std::string name = (alg == 0 ? "QuickRandom" : "QuickSelect");
        ofs << n << "," << name << "," << avgComp << "," << avgTime << "\n";
    }
    ofs.close();
}

/// Run one batch for Dual-Pivot variants (Random vs Select),
/// each repeated m times on random arrays of size n.
/// Append lines to “dual_results.csv” with columns:
///    n,algorithm,comparisons,time
void runDualOnce(int n, int m) {
    std::ofstream ofs("dual_results.csv", std::ios::app);
    // Two algorithms: 0=DualRandom, 1=DualSelect
    for (int alg = 0; alg < 2; ++alg) {
        double totalTime = 0.0;
        double totalComp = 0.0;
        for (int iter = 0; iter < m; ++iter) {
            std::vector<int> A(n);
            generateData(A);
            Counter cnt;
            auto start = std::chrono::high_resolution_clock::now();
            if (alg == 0) {
                DualPivotRandom(A, 0, n - 1, cnt, false);
            } else {
                DualPivotSelect(A, 0, n - 1, cnt, false);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            totalTime += diff.count();
            totalComp += cnt.comparisons;
        }
        double avgTime = totalTime / m;
        double avgComp = totalComp / m;
        std::string name = (alg == 0 ? "DualRandom" : "DualSelect");
        ofs << n << "," << name << "," << avgComp << "," << avgTime << "\n";
    }
    ofs.close();
}

/// Run worst-case experiment for base Quicksort (pivot = left) on sorted data.
/// Arrays are [0,1,2,...,n-1], which forces O(n^2) for that version.
/// Append lines to “worst_quick_results.csv” with columns:
///    n,comparisons,time
void runWorstQuick(int n) {
    // Build sorted array
    std::vector<int> A(n);
    for (int i = 0; i < n; ++i) A[i] = i;
    // Implement a basic Quicksort that always picks Pivot = A[left].
    Counter cnt;

    auto quickBasic = [&](auto&& self, int l, int r) -> void {
        if (l >= r) return;
        int pivot = A[l];
        int i = l + 1;
        for (int j = l + 1; j <= r; ++j) {
            cnt.comparisons++;
            if (A[j] < pivot) {
                std::swap(A[j], A[i]);
                i++;
            }
        }
        std::swap(A[l], A[i - 1]);
        int p = i - 1;
        self(self, l, p - 1);
        self(self, p + 1, r);
    };

    auto start = std::chrono::high_resolution_clock::now();
    quickBasic(quickBasic, 0, n - 1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::ofstream ofs("worst_quick_results.csv", std::ios::app);
    ofs << n << "," << cnt.comparisons << "," << diff.count() << "\n";
    ofs.close();
}

int main() {
    // Initialize/overwrite CSV files with headers
    {
        std::ofstream ofs1("quick_results.csv");
        ofs1 << "n,algorithm,comparisons,time\n";
        ofs1.close();
        std::ofstream ofs2("dual_results.csv");
        ofs2 << "n,algorithm,comparisons,time\n";
        ofs2.close();
        std::ofstream ofs3("worst_quick_results.csv");
        ofs3 << "n,comparisons,time\n";
        ofs3.close();
    }

    int m = 50;
    for (int n = 100; n <= 10000; n += 100) {
        runQuickOnce(n, m);
        runDualOnce(n, m);
        runWorstQuick(n);
    }
    return 0;
}
