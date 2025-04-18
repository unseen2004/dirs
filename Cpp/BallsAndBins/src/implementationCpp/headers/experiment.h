#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <vector>
#include <array>

std::vector<std::vector<std::array<int, 5>>> experiment();
void plot_results(const std::vector<std::vector<std::array<int, 5>>>& results);

#endif // EXPERIMENT_H