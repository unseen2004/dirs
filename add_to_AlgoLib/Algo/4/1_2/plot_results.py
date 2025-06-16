import pandas as pd
import matplotlib.pyplot as plt

# Read CSVs
df_inc = pd.read_csv('results_increasing.csv')
df_rand = pd.read_csv('results_random.csv')

# Plot average comparisons vs n
plt.figure()
plt.plot(df_inc['n'], df_inc['avg_comp'], marker='o', label='Increasing')
plt.plot(df_rand['n'], df_rand['avg_comp'], marker='o', label='Random')
plt.xlabel('n')
plt.ylabel('Average Comparisons')
plt.title('Average Comparisons per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_comparisons.png')

# Plot average pointer ops vs n
plt.figure()
plt.plot(df_inc['n'], df_inc['avg_ptr'], marker='o', label='Increasing')
plt.plot(df_rand['n'], df_rand['avg_ptr'], marker='o', label='Random')
plt.xlabel('n')
plt.ylabel('Average Pointer Operations')
plt.title('Average Pointer Ops per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_ptr_ops.png')

# Plot average height vs n
plt.figure()
plt.plot(df_inc['n'], df_inc['avg_height'], marker='o', label='Increasing')
plt.plot(df_rand['n'], df_rand['avg_height'], marker='o', label='Random')
plt.xlabel('n')
plt.ylabel('Average Height')
plt.title('Average Tree Height per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_height.png')

# Max comparisons
plt.figure()
plt.plot(df_inc['n'], df_inc['max_comp'], marker='o', label='Increasing')
plt.plot(df_rand['n'], df_rand['max_comp'], marker='o', label='Random')
plt.xlabel('n')
plt.ylabel('Max Comparisons')
plt.title('Max Comparisons per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_comparisons.png')

# Max pointer ops
plt.figure()
plt.plot(df_inc['n'], df_inc['max_ptr'], marker='o', label='Increasing')
plt.plot(df_rand['n'], df_rand['max_ptr'], marker='o', label='Random')
plt.xlabel('n')
plt.ylabel('Max Pointer Ops')
plt.title('Max Pointer Ops per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_ptr_ops.png')

# Max height
plt.figure()
plt.plot(df_inc['n'], df_inc['max_height'], marker='o', label='Increasing')
plt.plot(df_rand['n'], df_rand['max_height'], marker='o', label='Random')
plt.xlabel('n')
plt.ylabel('Max Height')
plt.title('Max Tree Height per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_height.png')

print("Plots saved as PNG files.")
