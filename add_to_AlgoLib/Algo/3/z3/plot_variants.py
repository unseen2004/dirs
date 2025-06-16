import pandas as pd
import matplotlib.pyplot as plt

# Read CSV: columns = [groupSize, n, k, comparisons, swaps]
df = pd.read_csv('variant_results.csv')

# Filter for median-case: k == n//2
median_df = df[df['k'] == (df['n'] // 2)]

# Compute average comparisons and swaps for each (groupSize, n)
group_comps = median_df.groupby(['groupSize', 'n'])['comparisons'].mean().reset_index()
group_swaps = median_df.groupby(['groupSize', 'n'])['swaps'].mean().reset_index()

# Plot: average comparisons vs n for each groupSize
plt.figure(figsize=(8,5))
for g in sorted(df['groupSize'].unique()):
    sub = group_comps[group_comps['groupSize'] == g]
    plt.plot(sub['n'], sub['comparisons'], label=f'g={g}')
plt.xlabel('n')
plt.ylabel('Avg Comparisons')
plt.title('Avg Comparisons vs n (k = n/2) for various group sizes')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('variant_comparisons.png')
plt.close()

# Plot: average swaps vs n for each groupSize
plt.figure(figsize=(8,5))
for g in sorted(df['groupSize'].unique()):
    sub = group_swaps[group_swaps['groupSize'] == g]
    plt.plot(sub['n'], sub['swaps'], label=f'g={g}')
plt.xlabel('n')
plt.ylabel('Avg Swaps')
plt.title('Avg Swaps vs n (k = n/2) for various group sizes')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('variant_swaps.png')
plt.close()

print("Plots saved as variant_comparisons.png and variant_swaps.png")
