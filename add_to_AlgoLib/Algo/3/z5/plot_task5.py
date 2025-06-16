import pandas as pd
import matplotlib.pyplot as plt

# === Quicksort Variants ===
qs_df = pd.read_csv('quick_results.csv')  # columns: n,algorithm,comparisons,time

plt.figure(figsize=(8,5))
for alg in qs_df['algorithm'].unique():
    sub = qs_df[qs_df['algorithm'] == alg]
    plt.plot(sub['n'], sub['comparisons'], label=alg)
plt.xlabel('n')
plt.ylabel('Avg Comparisons')
plt.title('Quicksort Variants: Avg Comparisons vs n')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('quick_comparisons.png')
plt.close()

plt.figure(figsize=(8,5))
for alg in qs_df['algorithm'].unique():
    sub = qs_df[qs_df['algorithm'] == alg]
    plt.plot(sub['n'], sub['time'], label=alg)
plt.xlabel('n')
plt.ylabel('Avg Time (s)')
plt.title('Quicksort Variants: Avg Time vs n')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('quick_time.png')
plt.close()

# === Dual-Pivot Quicksort Variants ===
dp_df = pd.read_csv('dual_results.csv')  # columns: n,algorithm,comparisons,time

plt.figure(figsize=(8,5))
for alg in dp_df['algorithm'].unique():
    sub = dp_df[dp_df['algorithm'] == alg]
    plt.plot(sub['n'], sub['comparisons'], label=alg)
plt.xlabel('n')
plt.ylabel('Avg Comparisons')
plt.title('Dual-Pivot Quicksort: Avg Comparisons vs n')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('dual_comparisons.png')
plt.close()

plt.figure(figsize=(8,5))
for alg in dp_df['algorithm'].unique():
    sub = dp_df[dp_df['algorithm'] == alg]
    plt.plot(sub['n'], sub['time'], label=alg)
plt.xlabel('n')
plt.ylabel('Avg Time (s)')
plt.title('Dual-Pivot Quicksort: Avg Time vs n')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('dual_time.png')
plt.close()

# === Worst-Case Base Quicksort ===
wc_df = pd.read_csv('worst_quick_results.csv')  # columns: n,comparisons,time

plt.figure(figsize=(8,5))
plt.plot(wc_df['n'], wc_df['comparisons'], label='WorstCaseQuick')
plt.xlabel('n')
plt.ylabel('Comparisons')
plt.title('Worst-Case Quicksort (Pivot=left): Comparisons vs n')
plt.grid(True)
plt.tight_layout()
plt.savefig('worst_quick_comp.png')
plt.close()

plt.figure(figsize=(8,5))
plt.plot(wc_df['n'], wc_df['time'], label='WorstCaseQuick')
plt.xlabel('n')
plt.ylabel('Time (s)')
plt.title('Worst-Case Quicksort (Pivot=left): Time vs n')
plt.grid(True)
plt.tight_layout()
plt.savefig('worst_quick_time.png')
plt.close()

print("Plots saved as quick_comparisons.png, quick_time.png, dual_comparisons.png, ")
print("dual_time.png, worst_quick_comp.png, worst_quick_time.png")
