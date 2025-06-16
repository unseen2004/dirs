import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def plot_for_small_ns(k, prefix='results_dual_vs_quick'):
    ns = [8, 32]
    gens = ['gen_random', 'gen_reverse', 'gen_sorted']
    algos = ['quicksort', 'dual_pivot_quicksort']
    for gen in gens:
        plt.figure(figsize=(8,5))
        for algo in algos:
            means_c, means_s = [], []
            for n in ns:
                fn = f"{prefix}/{algo}_{gen}_n{n}_k{k}.csv"
                if not os.path.exists(fn): continue
                df = pd.read_csv(fn)
                means_c.append(df.comparisons.mean())
                means_s.append(df.swaps.mean())
            plt.plot(ns[:len(means_c)], means_c, label=f'{algo} comparisons', marker='o')
            plt.plot(ns[:len(means_s)], means_s, label=f'{algo} swaps', marker='x', linestyle='--')
        plt.title(f'Comparisons/Swaps for {gen} input')
        plt.xlabel('n')
        plt.ylabel('count')
        plt.legend()
        plt.tight_layout()
        plt.savefig(f"{prefix}/plot_{gen}_smalln.png")
        plt.show()

if __name__ == "__main__":
    plot_for_small_ns(k=10)