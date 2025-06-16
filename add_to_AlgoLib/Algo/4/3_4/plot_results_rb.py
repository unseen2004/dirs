import pandas as pd
import matplotlib.pyplot as plt

# Read the two CSV files
df_inc_rb = pd.read_csv('results_increasing_rb.csv')
df_rand_rb = pd.read_csv('results_random_rb.csv')

# 1) Average Comparisons vs n
plt.figure()
plt.plot(df_inc_rb['n'], df_inc_rb['avg_comp'], marker='o', label='RB Increasing')
plt.plot(df_rand_rb['n'], df_rand_rb['avg_comp'], marker='o', label='RB Random')
plt.xlabel('n')
plt.ylabel('Average Comparisons')
plt.title('RB: Average Comparisons per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_comparisons_rb.png')

# 2) Average Pointer Ops vs n
plt.figure()
plt.plot(df_inc_rb['n'], df_inc_rb['avg_ptr'], marker='o', label='RB Increasing')
plt.plot(df_rand_rb['n'], df_rand_rb['avg_ptr'], marker='o', label='RB Random')
plt.xlabel('n')
plt.ylabel('Average Pointer Operations')
plt.title('RB: Average Pointer Ops per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_ptr_ops_rb.png')

# 3) Average Height vs n
plt.figure()
plt.plot(df_inc_rb['n'], df_inc_rb['avg_height'], marker='o', label='RB Increasing')
plt.plot(df_rand_rb['n'], df_rand_rb['avg_height'], marker='o', label='RB Random')
plt.xlabel('n')
plt.ylabel('Average Height')
plt.title('RB: Average Tree Height per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_height_rb.png')

# 4) Max Comparisons vs n
plt.figure()
plt.plot(df_inc_rb['n'], df_inc_rb['max_comp'], marker='o', label='RB Increasing')
plt.plot(df_rand_rb['n'], df_rand_rb['max_comp'], marker='o', label='RB Random')
plt.xlabel('n')
plt.ylabel('Max Comparisons')
plt.title('RB: Max Comparisons per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_comparisons_rb.png')

# 5) Max Pointer Ops vs n
plt.figure()
plt.plot(df_inc_rb['n'], df_inc_rb['max_ptr'], marker='o', label='RB Increasing')
plt.plot(df_rand_rb['n'], df_rand_rb['max_ptr'], marker='o', label='RB Random')
plt.xlabel('n')
plt.ylabel('Max Pointer Ops')
plt.title('RB: Max Pointer Ops per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_ptr_ops_rb.png')

# 6) Max Height vs n
plt.figure()
plt.plot(df_inc_rb['n'], df_inc_rb['max_height'], marker='o', label='RB Increasing')
plt.plot(df_rand_rb['n'], df_rand_rb['max_height'], marker='o', label='RB Random')
plt.xlabel('n')
plt.ylabel('Max Height')
plt.title('RB: Max Tree Height per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_height_rb.png')

print("RB plots saved as PNG files.")
