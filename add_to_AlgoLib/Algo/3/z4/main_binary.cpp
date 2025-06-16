#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include "BinarySearch.hpp"

/// Single‐run mode (demonstrate correctness on small data).
/// Creates a sorted array A = [1,2,...,n], then searches for v.
/// If n ≤ 30, prints the array and each comparison if verbose=true.
void runSingle(int n, int v) {
    std::vector<int> A(n);
    for (int i = 0; i < n; ++i) {
        A[i] = i + 1; // sorted 1..n
    }
    bool verbose = (n <= 30);
    if (verbose) {
        std::cout << "Array: ";
        for (int x : A) std::cout << x << (x == A.back() ? "\n" : " ");
        std::cout << "Search for v=" << v << "\n";
    }
    Counter cnt;
    auto start = std::chrono::high_resolution_clock::now();
    bool found = binarySearch(A, 0, n - 1, v, cnt, verbose);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << (found ? "Result: found" : "Result: not found")
              << ", comparisons=" << cnt.comparisons
              << ", time=" << diff.count() << "s\n";
}

/// Batch mode: for n = 1000, 2000, ..., 100000:
///  • measure comparisons & time for v = begin, middle, end, missing
///  • measure average over 50 random searches for v in the array
/// Output goes to "binary_results.csv" with columns:
///    n,scenario,comparisons,time
/// where scenario ∈ {begin,middle,end,missing,random}.
void runBatch() {
    // Overwrite (or create) CSV and write header
    {
        std::ofstream ofs("binary_results.csv");
        ofs << "n,scenario,comparisons,time\n";
    }

    // RNG for random‐element experiments
    std::mt19937 mt((unsigned)std::chrono::steady_clock::now().time_since_epoch().count());

    for (int n = 1000; n <= 100000; n += 1000) {
        // Build sorted array A = [1..n]
        std::vector<int> A(n);
        for (int i = 0; i < n; ++i) {
            A[i] = i + 1;
        }
        // Four fixed scenarios:
        struct Scenario { std::string name; int value; };
        Scenario scenarios[] = {
            {"begin",   A[0]},
            {"middle",  A[n/2]},
            {"end",     A[n-1]},
            {"missing", -1}
        };
        for (auto& sc : scenarios) {
            Counter cnt;
            auto start = std::chrono::high_resolution_clock::now();
            bool found = binarySearch(A, 0, n - 1, sc.value, cnt, false);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            // Append one line
            std::ofstream out("binary_results.csv", std::ios::app);
            out << n << "," << sc.name << "," << cnt.comparisons << "," << diff.count() << "\n";
            out.close();
        }
        // Random scenario: average over m = 50 random picks
        int m = 50;
        size_t totalComp = 0;
        double totalTime = 0.0;
        std::uniform_int_distribution<int> dist(0, n - 1);
        for (int iter = 0; iter < m; ++iter) {
            int idx = dist(mt);
            int v = A[idx];
            Counter cnt;
            auto start = std::chrono::high_resolution_clock::now();
            bool found = binarySearch(A, 0, n - 1, v, cnt, false);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            totalComp += cnt.comparisons;
            totalTime += diff.count();
        }
        double avgComp = static_cast<double>(totalComp) / m;
        double avgTime = totalTime / m;
        std::ofstream out("binary_results.csv", std::ios::app);
        out << n << ",random," << avgComp << "," << avgTime << "\n";
        out.close();
    }
}

int main(int argc, char* argv[]) {
    if (argc == 3) {
        int n = std::stoi(argv[1]);
        int v = std::stoi(argv[2]);
        runSingle(n, v);
    }
    else if (argc == 2 && std::string(argv[1]) == "--batch") {
        runBatch();
    }
    else {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " n v       (single run; array=1..n, search v)\n"
                  << "  " << argv[0] << " --batch  (run batch experiments)\n";
        return 1;
    }
    return 0;
}
