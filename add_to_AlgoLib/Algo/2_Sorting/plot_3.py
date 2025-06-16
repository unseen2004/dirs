import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

def plot_results(sizes, algos, k, prefix='results_natural', title_suffix='', saveas_prefix=''):
    colors = ['r', 'b']
    plt.figure(figsize=(10,6))
    for i, algo in enumerate(algos):
        mean_cmp, mean_swaps = [], []
        for n in sizes:
            fn = f"{prefix}/{algo}_n{n}_k{k}.csv"
            if not os.path.exists(fn): continue
            df = pd.read_csv(fn)
            mean_cmp.append(df.comparisons.mean())
            mean_swaps.append(df.swaps.mean())
        plt.plot(sizes[:len(mean_cmp)], mean_cmp, label=f'{algo} comparisons', color=colors[i], linestyle='-')
        plt.plot(sizes[:len(mean_swaps)], mean_swaps, label=f'{algo} swaps', color=colors[i], linestyle='--')
    plt.xlabel('n')
    plt.ylabel('count')
    plt.title(f'Comparisons and Swaps vs n {title_suffix}')
    plt.legend()
    plt.tight_layout()
    plt.savefig(f'{saveas_prefix}natural_merge_comps_swaps_vs_n.png')
    plt.show()

    plt.figure(figsize=(10,6))
    for i, algo in enumerate(algos):
        mean_cmp, mean_swaps = [], []
        for n in sizes:
            fn = f"{prefix}/{algo}_n{n}_k{k}.csv"
            if not os.path.exists(fn): continue
            df = pd.read_csv(fn)
            mean_cmp.append(df.comparisons.mean()/n)
            mean_swaps.append(df.swaps.mean()/n)
        plt.plot(sizes[:len(mean_cmp)], mean_cmp, label=f'{algo} c/n', color=colors[i], linestyle='-')
        plt.plot(sizes[:len(mean_swaps)], mean_swaps, label=f'{algo} s/n', color=colors[i], linestyle='--')
    plt.xlabel('n')
    plt.ylabel('normalized count')
    plt.title(f'Comparisons/n and Swaps/n vs n {title_suffix}')
    plt.legend()
    plt.tight_layout()
    plt.savefig(f'{saveas_prefix}natural_merge_comps_swaps_per_n_vs_n.png')
    plt.show()

if __name__ == "__main__":
    ns_small = list(range(10, 51, 10))
    ns_large = list(range(1000, 50001, 1000))
    algos = ['mergesort', 'natural_mergesort']
    plot_results(ns_small, algos, k=10, title_suffix="(small n)", saveas_prefix="smalln_")
    plot_results(ns_large, algos, k=10, title_suffix="(large n)", saveas_prefix="largen_")