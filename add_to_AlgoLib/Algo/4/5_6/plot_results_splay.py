import pandas as pd
import matplotlib.pyplot as plt

# Read CSVs
df_inc_sp = pd.read_csv('results_increasing_splay.csv')
df_rand_sp = pd.read_csv('results_random_splay.csv')

# 1) Average Comparisons vs n
plt.figure()
plt.plot(df_inc_sp['n'], df_inc_sp['avg_comp'], marker='o', label='Splay Increasing')
plt.plot(df_rand_sp['n'], df_rand_sp['avg_comp'], marker='o', label='Splay Random')
plt.xlabel('n')
plt.ylabel('Average Comparisons')
plt.title('Splay: Average Comparisons per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_comparisons_splay.png')

# 2) Average Pointer Ops vs n
plt.figure()
plt.plot(df_inc_sp['n'], df_inc_sp['avg_ptr'], marker='o', label='Splay Increasing')
plt.plot(df_rand_sp['n'], df_rand_sp['avg_ptr'], marker='o', label='Splay Random')
plt.xlabel('n')
plt.ylabel('Average Pointer Operations')
plt.title('Splay: Average Pointer Ops per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_ptr_ops_splay.png')

# 3) Average Height vs n
plt.figure()
plt.plot(df_inc_sp['n'], df_inc_sp['avg_height'], marker='o', label='Splay Increasing')
plt.plot(df_rand_sp['n'], df_rand_sp['avg_height'], marker='o', label='Splay Random')
plt.xlabel('n')
plt.ylabel('Average Height')
plt.title('Splay: Average Tree Height per Operation')
plt.legend()
plt.grid(True)
plt.savefig('avg_height_splay.png')

# 4) Max Comparisons vs n
plt.figure()
plt.plot(df_inc_sp['n'], df_inc_sp['max_comp'], marker='o', label='Splay Increasing')
plt.plot(df_rand_sp['n'], df_rand_sp['max_comp'], marker='o', label='Splay Random')
plt.xlabel('n')
plt.ylabel('Max Comparisons')
plt.title('Splay: Max Comparisons per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_comparisons_splay.png')

# 5) Max Pointer Ops vs n
plt.figure()
plt.plot(df_inc_sp['n'], df_inc_sp['max_ptr'], marker='o', label='Splay Increasing')
plt.plot(df_rand_sp['n'], df_rand_sp['max_ptr'], marker='o', label='Splay Random')
plt.xlabel('n')
plt.ylabel('Max Pointer Ops')
plt.title('Splay: Max Pointer Ops per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_ptr_ops_splay.png')

# 6) Max Height vs n
plt.figure()
plt.plot(df_inc_sp['n'], df_inc_sp['max_height'], marker='o', label='Splay Increasing')
plt.plot(df_rand_sp['n'], df_rand_sp['max_height'], marker='o', label='Splay Random')
plt.xlabel('n')
plt.ylabel('Max Height')
plt.title('Splay: Max Tree Height per Operation')
plt.legend()
plt.grid(True)
plt.savefig('max_height_splay.png')

print("Splay plots saved as PNG files.")
