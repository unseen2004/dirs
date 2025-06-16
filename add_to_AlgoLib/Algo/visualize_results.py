import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import os

def plot_operation_comparisons(n=500, num_experiments=5):
    """Plot comparison counts for each operation across multiple experiments"""
    
    plt.figure(figsize=(15, 10))
    
    colors = ['blue', 'red', 'green', 'orange', 'purple']
    
    for exp in range(1, num_experiments + 1):
        filename = f'experiment_n{n}_run{exp}.csv'
        if os.path.exists(filename):
            df = pd.read_csv(filename)
            plt.subplot(2, 3, exp)
            plt.plot(df['Operation'], df['Comparisons'], color=colors[exp-1], linewidth=1)
            plt.title(f'Experiment {exp} (n={n})')
            plt.xlabel('Operation Number')
            plt.ylabel('Number of Comparisons')
            plt.grid(True, alpha=0.3)
    
    # Combined plot
    plt.subplot(2, 3, 6)
    for exp in range(1, num_experiments + 1):
        filename = f'experiment_n{n}_run{exp}.csv'
        if os.path.exists(filename):
            df = pd.read_csv(filename)
            plt.plot(df['Operation'], df['Comparisons'], 
                    color=colors[exp-1], alpha=0.7, linewidth=1, 
                    label=f'Exp {exp}')
    
    plt.title(f'All Experiments Combined (n={n})')
    plt.xlabel('Operation Number')
    plt.ylabel('Number of Comparisons')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig(f'binomial_heap_experiments_n{n}.png', dpi=300, bbox_inches='tight')
    plt.show()

def plot_scaling_results():
    """Plot scaling results showing average cost per operation vs n"""
    
    if not os.path.exists('scaling_results.csv'):
        print("Scaling results file not found. Run the C++ program first.")
        return
    
    df = pd.read_csv('scaling_results.csv')
    
    plt.figure(figsize=(12, 8))
    
    plt.subplot(2, 2, 1)
    plt.plot(df['n'], df['Average_Cost_Per_Operation'], 'bo-', linewidth=2, markersize=4)
    plt.title('Average Cost Per Operation vs n')
    plt.xlabel('n (input size)')
    plt.ylabel('Average Comparisons per Operation')
    plt.grid(True, alpha=0.3)
    
    plt.subplot(2, 2, 2)
    plt.loglog(df['n'], df['Average_Cost_Per_Operation'], 'ro-', linewidth=2, markersize=4)
    plt.title('Average Cost Per Operation vs n (Log-Log Scale)')
    plt.xlabel('n (input size)')
    plt.ylabel('Average Comparisons per Operation')
    plt.grid(True, alpha=0.3)
    
    # Theoretical comparison with O(log n)
    plt.subplot(2, 2, 3)
    theoretical_log_n = np.log2(df['n']) * 0.5  # Scaled for comparison
    plt.plot(df['n'], df['Average_Cost_Per_Operation'], 'bo-', label='Experimental', linewidth=2)
    plt.plot(df['n'], theoretical_log_n, 'r--', label='O(log n) reference', linewidth=2)
    plt.title('Experimental vs Theoretical O(log n)')
    plt.xlabel('n (input size)')
    plt.ylabel('Comparisons')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Analysis of growth rate
    plt.subplot(2, 2, 4)
    growth_rate = np.diff(df['Average_Cost_Per_Operation']) / np.diff(df['n'])
    plt.plot(df['n'][1:], growth_rate, 'go-', linewidth=2, markersize=4)
    plt.title('Growth Rate of Average Cost')
    plt.xlabel('n (input size)')
    plt.ylabel('d(Cost)/dn')
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('binomial_heap_scaling_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()

def analyze_operation_phases(n=500):
    """Analyze different phases of operations (insert, union, extract)"""
    
    filename = f'experiment_n{n}_run1.csv'
    if not os.path.exists(filename):
        print(f"File {filename} not found. Run the C++ program first.")
        return
    
    df = pd.read_csv(filename)
    
    # Phase boundaries
    insert_phase_1 = n  # First n operations are inserts to H1
    insert_phase_2 = 2 * n  # Next n operations are inserts to H2
    union_phase = 2 * n + 1  # Union operation
    extract_phase_start = 2 * n + 2  # Extract operations start here
    
    plt.figure(figsize=(15, 10))
    
    # Full timeline
    plt.subplot(2, 2, 1)
    plt.plot(df['Operation'], df['Comparisons'], 'b-', linewidth=1)
    plt.axvline(x=insert_phase_1, color='red', linestyle='--', alpha=0.7, label='End of H1 inserts')
    plt.axvline(x=insert_phase_2, color='orange', linestyle='--', alpha=0.7, label='End of H2 inserts')
    plt.axvline(x=union_phase, color='green', linestyle='--', alpha=0.7, label='Union operation')
    plt.title(f'Complete Operation Timeline (n={n})')
    plt.xlabel('Operation Number')
    plt.ylabel('Number of Comparisons')
    plt.legend()
    plt.grid(True, alpha=0.3)
    
    # Insert phases
    plt.subplot(2, 2, 2)
    insert_ops = df[df['Operation'] <= insert_phase_2]
    plt.plot(insert_ops['Operation'], insert_ops['Comparisons'], 'bo-', markersize=2)
    plt.axvline(x=insert_phase_1, color='red', linestyle='--', alpha=0.7)
    plt.title('Insert Operations')
    plt.xlabel('Operation Number')
    plt.ylabel('Number of Comparisons')
    plt.grid(True, alpha=0.3)
    
    # Extract phase
    plt.subplot(2, 2, 3)
    extract_ops = df[df['Operation'] >= extract_phase_start]
    extract_ops_reindexed = extract_ops.copy()
    extract_ops_reindexed['Extract_Number'] = range(1, len(extract_ops) + 1)
    plt.plot(extract_ops_reindexed['Extract_Number'], extract_ops['Comparisons'], 'ro-', markersize=2)
    plt.title('Extract-Min Operations')
    plt.xlabel('Extract Operation Number')
    plt.ylabel('Number of Comparisons')
    plt.grid(True, alpha=0.3)
    
    # Statistics
    plt.subplot(2, 2, 4)
    phase_stats = {
        'Insert H1': df[df['Operation'] <= insert_phase_1]['Comparisons'].mean(),
        'Insert H2': df[(df['Operation'] > insert_phase_1) & (df['Operation'] <= insert_phase_2)]['Comparisons'].mean(),
        'Union': df[df['Operation'] == union_phase]['Comparisons'].iloc[0] if union_phase <= len(df) else 0,
        'Extract': df[df['Operation'] >= extract_phase_start]['Comparisons'].mean()
    }
    
    phases = list(phase_stats.keys())
    costs = list(phase_stats.values())
    colors = ['blue', 'cyan', 'green', 'red']
    
    bars = plt.bar(phases, costs, color=colors, alpha=0.7)
    plt.title('Average Comparisons by Operation Phase')
    plt.ylabel('Average Number of Comparisons')
    plt.xticks(rotation=45)
    
    # Add value labels on bars
    for bar, cost in zip(bars, costs):
        plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.1,
                f'{cost:.1f}', ha='center', va='bottom')
    
    plt.grid(True, alpha=0.3, axis='y')
    
    plt.tight_layout()
    plt.savefig(f'binomial_heap_phase_analysis_n{n}.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    return phase_stats

def main():
    print("=== Binomial Heap Visualization ===")
    
    # Check if result files exist
    if not any(os.path.exists(f'experiment_n500_run{i}.csv') for i in range(1, 6)):
        print("Experiment result files not found. Please run the C++ program first.")
        return
    
    # Plot individual experiments
    print("Generating plots for n=500 experiments...")
    plot_operation_comparisons(n=500, num_experiments=5)
    
    # Analyze operation phases
    print("Analyzing operation phases...")
    phase_stats = analyze_operation_phases(n=500)
    print("Phase statistics:", phase_stats)
    
    # Plot scaling results
    print("Generating scaling analysis...")
    plot_scaling_results()
    
    print("All visualizations completed!")

if __name__ == "__main__":
    main()