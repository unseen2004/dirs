import numpy as np
import matplotlib.pyplot as plt
import math

def load_maximum_load_data(filename, n_start=10000, n_step=10000):
    """
    Loads space-separated experiment results: each line has 50 integers
    (maximum load from 50 experiments). Returns:
      ns: list of n values (n_start, n_start + n_step, ...)
      data: list of lists, data[i] has 50 experiment values for that n.
    """
    data = []
    ns = []
    with open(filename, 'r') as f:
        current_n = n_start
        for line in f:
            values = line.strip().split()
            if not values:
                continue
            results = list(map(int, values))
            data.append(results)
            ns.append(current_n)
            current_n += n_step
    return ns, data

def load_insertion_sort_results(filename):
    """
    Each line format:
      n comp_1 comp_2 ... comp_50 shift_1 shift_2 ... shift_50
    Returns:
      ns: list of n values
      comps_all: list of lists of 50 comparisons
      shifts_all: list of lists of 50 shifts
    """
    ns, comps_all, shifts_all = [], [], []
    with open(filename, 'r') as f:
        for line in f:
            values = line.strip().split()
            if not values:
                continue
            vals = list(map(int, values))
            n = vals[0]
            comps = vals[1:51]
            shifts = vals[51:]
            ns.append(n)
            comps_all.append(comps)
            shifts_all.append(shifts)
    return ns, comps_all, shifts_all

def load_comm_interference_results(filename):
    """
    Each line format:
      n rounds_1 rounds_2 ... rounds_50
    Returns:
      ns: list of n values
      rounds_all: list of lists, each sublist has 50 round counts
    """
    ns, rounds_all = [], []
    with open(filename, 'r') as f:
        for line in f:
            values = line.strip().split()
            if not values:
                continue
            vals = list(map(int, values))
            n = vals[0]
            rounds_data = vals[1:]
            ns.append(n)
            rounds_all.append(rounds_data)
    return ns, rounds_all

def plot_max_load(ns_d1, data_d1, ns_d2, data_d2):
    """
    1. Plots the maximum load balanced allocation results for d=1 and d=2,
       including individual experiment points and averages.
    """
    plt.figure(figsize=(10, 6))

    # Plot for d=1
    for idx, n_val in enumerate(ns_d1):
        plt.scatter([n_val]*len(data_d1[idx]), data_d1[idx], color='blue', alpha=0.3, s=10)
    avg_d1 = [np.mean(exps) for exps in data_d1]
    plt.plot(ns_d1, avg_d1, color='blue', label='d=1 (avg)')

    # Plot for d=2
    for idx, n_val in enumerate(ns_d2):
        plt.scatter([n_val]*len(data_d2[idx]), data_d2[idx], color='red', alpha=0.3, s=10)
    avg_d2 = [np.mean(exps) for exps in data_d2]
    plt.plot(ns_d2, avg_d2, color='red', label='d=2 (avg)')

    plt.xlabel('n')
    plt.ylabel('Maximum load')
    plt.title('1. Maximum Load Balanced Allocation')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_load_ratios(ns_d1, data_d1, data_d2):
    """
    2. Plot l1(n)/f1(n) and l2(n)/f2(n)
       where l1(n) and l2(n) are averaged max loads for d=1 and d=2.
       f1(n)=ln(n)/lnln(n) and f2(n)=lnln(n)/ln(2).
    """
    l1 = np.array([np.mean(x) for x in data_d1], dtype=float)
    l2 = np.array([np.mean(x) for x in data_d2], dtype=float)
    ns_arr = np.array(ns_d1, dtype=float)

    f1 = np.log(ns_arr) / np.log(np.log(ns_arr))
    f2 = np.log(np.log(ns_arr)) / np.log(2.0)

    ratio_d1 = l1 / f1
    ratio_d2 = l2 / f2

    plt.figure(figsize=(10, 6))
    plt.plot(ns_arr, ratio_d1, marker='o', color='blue', label='l1(n)/f1(n)')
    plt.plot(ns_arr, ratio_d2, marker='o', color='red', label='l2(n)/f2(n)')
    plt.xlabel('n')
    plt.ylabel('Ratio')
    plt.title('2. Ratios for d=1, d=2')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_insertion_sort_comparisons(ns_sort, comps_all):
    """
    3. Plots the number of comparisons in individual iterations and
       the average number of comparisons, cmp(n).
    """
    cmp_means = [np.mean(c) for c in comps_all]
    plt.figure(figsize=(10, 6))
    for idx, n_val in enumerate(ns_sort):
        plt.scatter([n_val]*len(comps_all[idx]), comps_all[idx], color='blue', alpha=0.3, s=10)
    plt.plot(ns_sort, cmp_means, color='blue', label='Average comparisons cmp(n)')
    plt.xlabel('n')
    plt.ylabel('Number of comparisons')
    plt.title('3. Insertion Sort Comparisons')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_insertion_sort_rearrangements(ns_sort, shifts_all):
    """
    4. Plots the number of shifts in individual iterations and
       the average number of shifts, s(n).
    """
    shifts_means = [np.mean(s) for s in shifts_all]
    plt.figure(figsize=(10, 6))
    for idx, n_val in enumerate(ns_sort):
        plt.scatter([n_val]*len(shifts_all[idx]), shifts_all[idx], color='red', alpha=0.3, s=10)
    plt.plot(ns_sort, shifts_means, color='red', label='Average shifts s(n)')
    plt.xlabel('n')
    plt.ylabel('Number of rearrangements (shifts)')
    plt.title('4. Insertion Sort Rearrangements')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_cmp_over_n(ns_sort, comps_all):
    """
    5. Plots cmp(n)/n and cmp(n)/n^2 as functions of n.
    """
    cmp_means = [np.mean(c) for c in comps_all]
    cmp_over_n = [cmp_means[i]/ns_sort[i] for i in range(len(ns_sort))]
    cmp_over_n2 = [cmp_means[i]/(ns_sort[i]**2) for i in range(len(ns_sort))]

    plt.figure(figsize=(10, 6))
    plt.plot(ns_sort, cmp_over_n, marker='o', color='blue', label='cmp(n)/n')
    plt.plot(ns_sort, cmp_over_n2, marker='o', color='red', label='cmp(n)/n^2')
    plt.xlabel('n')
    plt.ylabel('Value')
    plt.title('5. cmp(n)/n and cmp(n)/n^2')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_shifts_over_n(ns_sort, shifts_all):
    """
    6. Plots s(n)/n and s(n)/n^2 as functions of n.
    """
    shifts_means = [np.mean(s) for s in shifts_all]
    s_over_n = [shifts_means[i]/ns_sort[i] for i in range(len(ns_sort))]
    s_over_n2 = [shifts_means[i]/(ns_sort[i]**2) for i in range(len(ns_sort))]

    plt.figure(figsize=(10, 6))
    plt.plot(ns_sort, s_over_n, marker='o', color='blue', label='s(n)/n')
    plt.plot(ns_sort, s_over_n2, marker='o', color='red', label='s(n)/n^2')
    plt.xlabel('n')
    plt.ylabel('Value')
    plt.title('6. s(n)/n and s(n)/n^2')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def plot_comm_rounds(ns_comm, rounds_all, title_str='Communication Interference'):
    """
    7 & 8. Plots the number of rounds needed in individual repetitions and
           the average t(n) as a function of n.
    """
    rounds_mean = [np.mean(r) for r in rounds_all]
    plt.figure(figsize=(10, 6))
    for idx, n_val in enumerate(ns_comm):
        plt.scatter([n_val]*len(rounds_all[idx]), rounds_all[idx], alpha=0.3, s=10)
    plt.plot(ns_comm, rounds_mean, color='purple', label='Average t(n)')
    plt.xlabel('n')
    plt.ylabel('Number of rounds')
    plt.title(title_str)
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.show()

def main():
    # Load data for maximum load balanced allocation (d=1 and d=2)
    ns_d1, data_d1 = load_maximum_load_data("to_plot/max_load_results1.txt", n_start=10000, n_step=10000)
    ns_d2, data_d2 = load_maximum_load_data("to_plot/max_load_results2.txt", n_start=10000, n_step=10000)

    # 1. Plot maximum load balanced allocations
    plot_max_load(ns_d1, data_d1, ns_d2, data_d2)

    # 2. Plot l1(n)/f1(n) and l2(n)/f2(n)
    plot_load_ratios(ns_d1, data_d1, data_d2)

    # Load insertion sort results
    ns_sort, comps_all, shifts_all = load_insertion_sort_results("to_plot/insertion_sort_results.txt")

    # 3. Plot insertion sort comparisons
    plot_insertion_sort_comparisons(ns_sort, comps_all)

    # 4. Plot insertion sort rearrangements
    plot_insertion_sort_rearrangements(ns_sort, shifts_all)

    # 5. Plot cmp(n)/n and cmp(n)/n^2 as functions of n
    plot_cmp_over_n(ns_sort, comps_all)

    # 6. Plot s(n)/n and s(n)/n^2 as functions of n
    plot_shifts_over_n(ns_sort, shifts_all)

    # 7. Communication interference p=0.5
    ns_comm1, rounds_all_1 = load_comm_interference_results("to_plot/comm_interference_results1.txt")
    plot_comm_rounds(ns_comm1, rounds_all_1, title_str='7. Comm Interference (p=0.5)')

    # 8. Communication interference p=0.1
    ns_comm2, rounds_all_2 = load_comm_interference_results("to_plot/comm_interference_results2.txt")
    plot_comm_rounds(ns_comm2, rounds_all_2, title_str='8. Comm Interference (p=0.1)')

if __name__ == "__main__":
    main()