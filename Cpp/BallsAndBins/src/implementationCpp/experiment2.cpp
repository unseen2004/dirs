#include "experiment2.h"
#include <iostream>
#include <vector>
#include "random.h"
#include <algorithm>


data::data(int n, const std::vector<int> &comp, const std::vector<int> &shift)
    : n(n), n_comp(comp), n_shift(shift) {
}

data2::data2(int n, const std::vector<int> &rounds)
    : n(n), min_rounds(rounds) {
}

std::vector<std::vector<int> > maximum_load_balanced_allocation(int d) {
    std::vector<std::vector<int> > results;

    for (int n = 10000; n <= 1000000; n += 10000) {
        std::vector<int> experiments;
        std::cout << n << '\n';

        // Perform 50 experiments
        for (int k = 0; k < 50; ++k) {
            std::vector<int> bins(n, 0);

            // Place n balls into bins
            for (int i = 0; i < n; ++i) {
                // Pick d random bins
                std::vector<int> chosenBins;
                chosenBins.reserve(d);
                for (int j = 0; j < d; ++j) {
                    chosenBins.push_back(Random::get(0, n - 1));
                }

                // Find the chosen bin with the smallest current load
                auto it = std::min_element(chosenBins.begin(), chosenBins.end(),
                                           [&](int a, int b) {
                                               return bins[a] < bins[b];
                                           }
                );

                // Increment that bin
                bins[*it]++;
            }

            // Record the maximum load in this experiment
            auto maxLoad = *std::max_element(bins.begin(), bins.end());
            experiments.push_back(maxLoad);
        }
        results.push_back(experiments);
    }
    return results;
}


std::vector<int> generate_random_permutation(int n) {
    std::vector<int> A(n);

    for (int i = 0; i < n; ++i) {
        A[i] = i + 1;
    }

    // Perform the Fisher-Yates shuffle
    for (int i = n - 1; i > 0; --i) {
        int j = Random::get(0, i);
        std::swap(A[i], A[j]);
    }

    return A;
}

void insertion_sort(std::vector<int> &arr, std::vector<int> &out_comp, std::vector<int> &out_shift) {
    int n = arr.size();
    int comparisons = 0;
    int shifts = 0;

    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        // Count the comparison for the while loop condition
        while (j >= 0 && arr[j] > key) {
            comparisons++;
            arr[j + 1] = arr[j];
            shifts++;
            --j;
        }
        // Count the final comparison where the while loop condition fails
        if (j >= 0) {
            comparisons++;
        }
        arr[j + 1] = key;
        shifts++;
    }

    out_comp.push_back(comparisons);
    out_shift.push_back(shifts);
}

std::vector<data> test_insertion_sort() {
    std::vector<data> results{};

    for (int n{100}; n <= 10000; n += 100) {
        std::vector<int> n_comp{};
        std::vector<int> n_shift{};
        for (int k{}; k < 50; ++k) {
            auto arr = generate_random_permutation(n);
            insertion_sort(arr, n_comp, n_shift);
        }
        results.emplace_back(n, n_comp, n_shift);
    }
    return results;
}

std::vector<data2> communication_with_interference(double p) {
    std::vector<data2> results{};

    for (int n{1000}; n <= 100000; n += 1000) {
        std::vector<int> min_rounds{};
        std::cout << n << "\n";
        // experiment
        for (int k{}; k < 50; ++k) {
            std::vector<bool> stations(n, false);
            int n_received{};
            int rounds{};
            while (n_received < n) {
                // round
                ++rounds;
                for (int i{}; i < n; ++i) {
                    if (!stations[i]) {
                        if (Random::get(0.0, 1.0) < p) {
                                stations[i] = true;
                                ++n_received;
                        }
                    }
                }
            }
            min_rounds.push_back(rounds);
        }
        results.emplace_back(n, min_rounds);
    }
    return results;
}


