#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <vector>
#include <array>
#include <string>
#include "experiment2.h"

void save_results_to_file(const std::vector<std::vector<std::array<int, 5>>>& results, const std::string& filename);
std::vector<std::vector<std::array<int, 5>>> read_results_from_file(const std::string& filename);

void save_maximum_load_balanced_allocation_results(const std::vector<std::vector<int>>& results, const std::string& filename);
void save_insertion_sort_results(const std::vector<data>& results, const std::string& filename);
void save_communication_with_interference_results(const std::vector<data2>& results, const std::string& filename);

#endif // FILE_OPERATIONS_H