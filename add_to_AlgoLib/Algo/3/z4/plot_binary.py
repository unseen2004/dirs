import pandas as pd
import matplotlib.pyplot as plt

# Read CSV: columns = [n, scenario, comparisons, time]
df = pd.read_csv('binary_results.csv')

# We expect scenarios = ['begin','middle','end','missing','random']
scenarios = ['begin', 'middle', 'end', 'missing', 'random']

# Plot: Comparisons vs n
plt.figure(figsize=(8,5))
for sc in scenarios:
    sub = df[df['scenario'] == sc]
    plt.plot(sub['n'], sub['comparisons'], label=sc)
plt.xlabel('n')
plt.ylabel('Comparisons')
plt.title('Binary Search: Comparisons vs n')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('binary_comparisons.png')
plt.close()

# Plot: Time vs n
plt.figure(figsize=(8,5))
for sc in scenarios:
    sub = df[df['scenario'] == sc]
    plt.plot(sub['n'], sub['time'], label=sc)
plt.xlabel('n')
plt.ylabel('Time (seconds)')
plt.title('Binary Search: Time vs n')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('binary_time.png')
plt.close()

print("Plots saved as binary_comparisons.png and binary_time.png")
