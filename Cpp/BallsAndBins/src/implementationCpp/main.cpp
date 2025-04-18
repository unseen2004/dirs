#include "experiment2.h"
#include "file_operations.h"

int main() {
    auto max_load_results1 = maximum_load_balanced_allocation(1);
    save_maximum_load_balanced_allocation_results({max_load_results1}, "max_load_results1.txt");

    auto max_load_results2 = maximum_load_balanced_allocation(2);
    save_maximum_load_balanced_allocation_results({max_load_results2}, "max_load_results2.txt");

    auto insertion_sort_results = test_insertion_sort();
    save_insertion_sort_results(insertion_sort_results, "insertion_sort_results.txt");

    auto comm_interference_results1 = communication_with_interference(0.5);
    save_communication_with_interference_results({comm_interference_results1}, "comm_interference_results1.txt");

    auto comm_interference_results2 = communication_with_interference(0.1);
    save_communication_with_interference_results({comm_interference_results2}, "comm_interference_results2.txt");

    return 0;
}