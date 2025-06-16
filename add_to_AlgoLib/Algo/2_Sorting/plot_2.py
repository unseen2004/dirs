import pandas as pd
import matplotlib.pyplot as plt
import glob
import os

def plot_experiment(sizes, algos, k, prefix='results', title_suffix='', saveas_prefix=''):
    colors = ['r', 'g', 'b', 'm', 'c']
    plt.figure(figsize=(12,8))
    for i, algo in enumerate(algos):
        means_c, means_s = [], []
        for n in sizes:
            fn = f"{prefix}/{algo}_n{n}_k{k}.csv"
            if not os.path.exists(fn): continue
            df = pd.read_csv(fn)
            means_c.append(df.comparisons.mean())
            means_s.append(df.swaps.mean())
        plt.plot(sizes[:len(means_c)], means_c, label=f'{algo} comparisons', color=colors[i], linestyle='-')
        plt.plot(sizes[:len(means_s)], means_s, label=f'{algo} swaps', color=colors[i], linestyle='--')
    plt.xlabel('n')
    plt.ylabel('count')
    plt.title(f'Comparisons and Swaps vs n {title_suffix}')
    plt.legend()
    plt.savefig(f'{saveas_prefix}comp_swaps_vs_n.png')
    plt.show()

    # Now c/n and s/n
    plt.figure(figsize=(12,8))
    for i, algo in enumerate(algos):
        means_c, means_s = [], []
        for n in sizes:
            fn = f"{prefix}/{algo}_n{n}_k{k}.csv"
            if not os.path.exists(fn): continue
            df = pd.read_csv(fn)
            means_c.append(df.comparisons.mean()/n)
            means_s.append(df.swaps.mean()/n)
        plt.plot(sizes[:len(means_c)], means_c, label=f'{algo} c/n', color=colors[i], linestyle='-')
        plt.plot(sizes[:len(means_s)], means_s, label=f'{algo} s/n', color=colors[i], linestyle='--')
    plt.xlabel('n')
    plt.ylabel('normalized count')
    plt.title(f'Comparisons/n and Swaps/n vs n {title_suffix}')
    plt.legend()
    plt.savefig(f'{saveas_prefix}comp_swaps_per_n_vs_n.png')
    plt.show()

if __name__ == "__main__":
    # Small n
    ns = list(range(10, 51, 10))
    algos = ['insertion_sort', 'quicksort', 'hybrid_quicksort']
    plot_experiment(ns, algos, k=10, title_suffix="(small n)", saveas_prefix='smalln_')

    # Large n
    ns = list(range(1000, 50001, 1000))
    algos = ['quicksort', 'hybrid_quicksort']
    plot_experiment(ns, algos, k=10, title_suffix="(large n)", saveas_prefix='largen_')