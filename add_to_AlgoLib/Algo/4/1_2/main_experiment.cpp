#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "BST.hpp"
#include "Random.hpp"

// Structure to record metrics per operation
struct OpMetrics {
    uint64_t comp;
    uint64_t ptr;
    int height;
};

int main() {
    std::vector<int> ns = {10000,20000,30000,40000,50000,60000,70000,80000,90000,100000};
    const int trials = 20;

    std::ofstream ofs_inc("results_increasing.csv");
    std::ofstream ofs_rand("results_random.csv");
    // Header: n,avg_comp,avg_ptr,avg_height,max_comp,max_ptr,max_height
    ofs_inc << "n,avg_comp,avg_ptr,avg_height,max_comp,max_ptr,max_height\n";
    ofs_rand << "n,avg_comp,avg_ptr,avg_height,max_comp,max_ptr,max_height\n";

    for (int n : ns) {
        std::cout << "Running trials for n = " << n << "...\n";
        // Case A: increasing insert, random delete
        uint64_t total_comp = 0, total_ptr = 0;
        uint64_t max_comp = 0, max_ptr = 0;
        uint64_t total_height = 0; int max_height = 0;

        for (int t = 0; t < trials; ++t) {
            BST tree;
            std::vector<int> keys(n);
            for (int i = 0; i < n; ++i) keys[i] = i + 1;

            // Insert increasing
            for (int k : keys) {
                tree.resetCounters();
                tree.insert(k);
                total_comp += COMP_COUNT;
                total_ptr += PTR_COUNT;
                max_comp = std::max(max_comp, COMP_COUNT);
                max_ptr = std::max(max_ptr, PTR_COUNT);
                total_height += tree.currentHeight;
                max_height = std::max(max_height, tree.currentHeight);
            }
            // Delete in random order
            std::shuffle(keys.begin(), keys.end(), Random::mt);
            for (int k : keys) {
                tree.resetCounters();
                tree.remove(k);
                total_comp += COMP_COUNT;
                total_ptr += PTR_COUNT;
                max_comp = std::max(max_comp, COMP_COUNT);
                max_ptr = std::max(max_ptr, PTR_COUNT);
                total_height += tree.currentHeight;
                max_height = std::max(max_height, tree.currentHeight);
            }
        }
        uint64_t ops = static_cast<uint64_t>(n) * 2 * trials; // inserts + deletes
        ofs_inc << n << ","
                << (double)total_comp / ops << ","
                << (double)total_ptr / ops << ","
                << (double)total_height / ops << ","
                << max_comp << ","
                << max_ptr << ","
                << max_height << "\n";

        // Case B: random insert, random delete
        total_comp = total_ptr = 0;
        max_comp = max_ptr = 0;
        total_height = 0; max_height = 0;
        for (int t = 0; t < trials; ++t) {
            BST tree;
            std::vector<int> keys(n);
            for (int i = 0; i < n; ++i) keys[i] = i + 1;
            std::shuffle(keys.begin(), keys.end(), Random::mt);
            // Insert random
            for (int k : keys) {
                tree.resetCounters();
                tree.insert(k);
                total_comp += COMP_COUNT;
                total_ptr += PTR_COUNT;
                max_comp = std::max(max_comp, COMP_COUNT);
                max_ptr = std::max(max_ptr, PTR_COUNT);
                total_height += tree.currentHeight;
                max_height = std::max(max_height, tree.currentHeight);
            }
            // Delete random
            std::shuffle(keys.begin(), keys.end(), Random::mt);
            for (int k : keys) {
                tree.resetCounters();
                tree.remove(k);
                total_comp += COMP_COUNT;
                total_ptr += PTR_COUNT;
                max_comp = std::max(max_comp, COMP_COUNT);
                max_ptr = std::max(max_ptr, PTR_COUNT);
                total_height += tree.currentHeight;
                max_height = std::max(max_height, tree.currentHeight);
            }
        }
        ops = static_cast<uint64_t>(n) * 2 * trials;
        ofs_rand << n << ","
                 << (double)total_comp / ops << ","
                 << (double)total_ptr / ops << ","
                 << (double)total_height / ops << ","
                 << max_comp << ","
                 << max_ptr << ","
                 << max_height << "\n";
    }

    ofs_inc.close();
    ofs_rand.close();

    std::cout << "Experiment data written to results_increasing.csv and results_random.csv" << std::endl;
    return 0;
}

