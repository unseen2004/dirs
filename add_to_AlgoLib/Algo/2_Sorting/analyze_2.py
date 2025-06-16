import pandas as pd
import glob
import re

result = []
for fn in glob.glob("threshold_results/hybrid_th*_n*_k*.csv"):
    t = int(re.search(r'th(\d+)_n', fn).group(1))
    n = int(re.search(r'_n(\d+)_k', fn).group(1))
    df = pd.read_csv(fn)
    avg_cmp = df.comparisons.mean()
    avg_swaps = df.swaps.mean()
    result.append((t, n, avg_cmp, avg_swaps))

df = pd.DataFrame(result, columns=['threshold', 'n', 'avg_cmp', 'avg_swaps'])
pivot = df.pivot(index='n', columns='threshold', values='avg_cmp')
print("Average comparisons for each threshold and n:")
print(pivot)
best = df.groupby('threshold')['avg_cmp'].mean().sort_values()
print("\nMean avg comparisons by threshold:")
print(best)
print("\nBest threshold is:", best.idxmin())