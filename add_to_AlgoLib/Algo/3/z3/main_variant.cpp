#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include "SelectVariant.hpp"
#include "Random.hpp"

/// Fill A with random ints in [0, 2n−1].
void generateData(std::vector<int>& A) {
    size_t n = A.size();
    for (size_t i = 0; i < n; ++i) {
        A[i] = Random::get<int>(0, static_cast<int>(2 * n - 1));
    }
}

/// Run one instance of the variant‐Select with a given group size:
/// - n: array size
/// - k: 1-based order statistic
/// - groupSize: 3, 5, 7, or 9
/// - verbose: if true (n≤30), print array states
///
/// Prints a summary line to stdout and appends one row to “variant_results.csv”:
///   groupSize,n,k,comparisons,swaps
void runOnceVariant(size_t n, size_t k, size_t groupSize, bool verbose) {
    std::vector<int> A(n), B(n);
    generateData(A);
    B = A;  // keep a copy for possible verification

    Counter cnt;
    auto start = std::chrono::high_resolution_clock::now();
    int result = SelectVariant(A, 0, n - 1, k - 1, cnt, verbose, groupSize);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    // Print a summary to stdout (optional)
    std::cout << "Group=" << groupSize
              << ", n=" << n
              << ", k=" << k
              << ", result=" << result
              << ", comps=" << cnt.comparisons
              << ", swaps=" << cnt.swaps
              << ", time=" << diff.count() << "s\n";

    // Append one CSV line: groupSize,n,k,comparisons,swaps
    std::ofstream ofs("variant_results.csv", std::ios::app);
    ofs << groupSize << "," << n << "," << k << ","
        << cnt.comparisons << "," << cnt.swaps << "\n";
    ofs.close();
}

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--batch") {
        std::vector<size_t> groupSizes = {3, 5, 7, 9};
        const size_t m = 50;                     // repeats per (n,k,groupSize)
        std::vector<size_t> ns;
        for (size_t i = 100; i <= 50000; i += 100) ns.push_back(i);

        // Create or overwrite CSV with header
        {
            std::ofstream ofs("variant_results.csv");
            ofs << "groupSize,n,k,comparisons,swaps\n";
        }

        for (size_t g : groupSizes) {
            for (size_t n : ns) {
                std::vector<size_t> ks = {1, n / 2, n};
                for (size_t k : ks) {
                    for (size_t iter = 0; iter < m; ++iter) {
                        runOnceVariant(n, k, g, false);
                    }
                }
            }
        }
    }
    else {
        std::cerr << "Usage: " << argv[0] << " --batch\n";
        return 1;
    }
    return 0;
}
