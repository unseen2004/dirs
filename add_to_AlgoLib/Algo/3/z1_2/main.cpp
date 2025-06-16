#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include "Select.hpp"
#include "RandomizedSelect.hpp"
#include "Random.hpp"

/// Fill A with random ints in [0, 2n−1].
void generateData(std::vector<int>& A) {
    size_t n = A.size();
    for (size_t i = 0; i < n; ++i) {
        A[i] = Random::get<int>(0, static_cast<int>(2 * n - 1));
    }
}

/// Run exactly one instance of either Select or RandomizedSelect.
/// - n: size of A
/// - k: 1-based order‐statistic
/// - useRandomized: if true, run RandomizedSelect; else deterministic Select
/// - verbose: if n ≤ 30, print intermediate array states
///
/// Prints to stdout:
///   • “Select, n=…, k=…, result=…, comps=…, swaps=…, time=…s”
///   • (If verbose) initial array, each partition state, final array, sorted array, k-th sorted.
/// Appends one line of CSV: “n,k,comparisons,swaps” to either
/// “select_results.csv” or “randomized_results.csv”.
void runOnce(size_t n, size_t k, bool useRandomized, bool verbose) {
    std::vector<int> A(n), B(n);
    generateData(A);
    B = A;  // keep an unsorted copy

    if (verbose) {
        std::cout << "Initial array:\n";
        for (int v : A) std::cout << v << " ";
        std::cout << "\n\n";
    }

    Counter cnt;
    int result;
    auto start = std::chrono::high_resolution_clock::now();

    if (useRandomized) {
        result = RandomizedSelect(A, 0, n - 1, k - 1, cnt, verbose);
    } else {
        result = Select(A, 0, n - 1, k - 1, cnt, verbose);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    if (verbose) {
        std::cout << "\nFinal array:\n";
        for (int v : A) std::cout << v << " ";
        std::cout << "\n\n";
    }

    std::cout << (useRandomized ? "RandomizedSelect" : "Select")
              << ", n=" << n << ", k=" << k
              << ", result=" << result
              << ", comps=" << cnt.comparisons
              << ", swaps=" << cnt.swaps
              << ", time=" << diff.count() << "s\n";

    if (verbose) {
        std::sort(B.begin(), B.end());
        std::cout << "\nSorted array:\n";
        for (int v : B) std::cout << v << " ";
        std::cout << "\n\nK-th element in sorted: " << B[k - 1] << "\n";
    }

    // Append one CSV line for plotting later
    std::ofstream ofs(useRandomized ? "randomized_results.csv" : "select_results.csv",
                      std::ios::app);
    ofs << n << "," << k << "," << cnt.comparisons << "," << cnt.swaps << "\n";
    ofs.close();

}

int main(int argc, char* argv[]) {
    if (argc == 4) {
        // Single-run mode: <program> n k alg
        size_t n       = std::stoul(argv[1]);
        size_t k       = std::stoul(argv[2]);
        int    alg     = std::stoi(argv[3]); // 0 = Select, 1 = RandomizedSelect
        bool   verbose = (n <= 30);

        runOnce(n, k, (alg == 1), verbose);
    }
    else if (argc == 2 && std::string(argv[1]) == "--batch") {
        // Batch mode: n = 100, 200, …, 50000, each with k = 1, n/2, n, 50 repetitions
        const size_t m = 50;
        std::vector<size_t> ns;
        for (size_t i = 100; i <= 50000; i += 100) {
            ns.push_back(i);
        }

        // Create/clear CSV files with headers:
        {
            std::ofstream ofs("select_results.csv");
            ofs << "n,k,comparisons,swaps\n";
        }
        {
            std::ofstream ofs("randomized_results.csv");
            ofs << "n,k,comparisons,swaps\n";
        }

        for (size_t nVal : ns) {
            std::vector<size_t> ks = {1, nVal / 2, nVal};
            for (size_t kVal : ks) {
                for (size_t iter = 0; iter < m; ++iter) {
                    runOnce(nVal, kVal, false, false);
                    runOnce(nVal, kVal, true, false);
                }
            }
        }
    }
    else {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " n k alg(0=Select,1=Randomized)\n"
                  << "  " << argv[0] << " --batch\n";
        return 1;
    }
    return 0;
}
