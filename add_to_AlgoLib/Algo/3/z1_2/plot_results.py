import pandas as pd
import matplotlib.pyplot as plt

# Read the two CSV files (each has columns: n,k,comparisons,swaps).
select_df  = pd.read_csv('select_results.csv')
random_df  = pd.read_csv('randomized_results.csv')

# Tag them so we can plot both on the same axes.
select_df['algorithm']   = 'Select'
random_df['algorithm']   = 'RandomizedSelect'

# Combine into one DataFrame for easy grouping.
df = pd.concat([select_df, random_df])

# Filter for median‐case (k = n//2).
median_df = df[df['k'] == (df['n'] // 2)]

# Compute average comparisons and swaps for each (algorithm, n).
median_group_comps = median_df.groupby(['algorithm','n'])['comparisons'].mean().reset_index()
median_group_swaps = median_df.groupby(['algorithm','n'])['swaps'].mean().reset_index()

# Plot average comparisons vs n
plt.figure(figsize=(8,5))
for alg in ['Select','RandomizedSelect']:
    sub = median_group_comps[median_group_comps['algorithm'] == alg]
    plt.plot(sub['n'], sub['comparisons'], label=alg)
plt.xlabel('n')
plt.ylabel('Average Comparisons')
plt.title('Average Comparisons vs n (k = n/2)')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('comparisons_plot.png')
plt.close()

# Plot average swaps vs n
plt.figure(figsize=(8,5))
for alg in ['Select','RandomizedSelect']:
    sub = median_group_swaps[median_group_swaps['algorithm'] == alg]
    plt.plot(sub['n'], sub['swaps'], label=alg)
plt.xlabel('n')
plt.ylabel('Average Swaps')
plt.title('Average Swaps vs n (k = n/2)')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('swaps_plot.png')
plt.close()

print("Plots saved as comparisons_plot.png and swaps_plot.png")
