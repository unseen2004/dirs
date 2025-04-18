#ifndef EXPERIMENT2_H
#define EXPERIMENT2_H

#include <vector>

std::vector<std::vector<int>> maximum_load_balanced_allocation(int d);

struct data {
    int n;
    std::vector<int> n_comp;
    std::vector<int> n_shift;

    data(int n, const std::vector<int> &comp, const std::vector<int> &shift);
};

std::vector<int> generate_random_permutation(int n);

void insertion_sort(std::vector<int> &arr, std::vector<int> &out_comp, std::vector<int> &out_shift);

std::vector<data> test_insertion_sort();

struct data2 {
    int n;
    std::vector<int> min_rounds;

    data2(int n, const std::vector<int> &rounds);
};

std::vector<data2> communication_with_interference(double p);

#endif // EXPERIMENT2_H