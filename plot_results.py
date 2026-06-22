import os
import pandas as pd
import matplotlib.pyplot as plt

def main():
    csv_in = 'resultados.csv'
    csv_out = 'results.csv'

    if not os.path.exists(csv_out) and os.path.exists(csv_in):
        print(f"Translating {csv_in} to {csv_out}...")
        df = pd.read_csv(csv_in)
        
        column_mapping = {
            'metodo': 'method',
            'nhilos': 'threads',
            'ndatos': 'data_size',
            'tiempo(s)': 'time(s)',
            'cpu(%)': 'cpu(%)',
            'ram(KB)': 'ram(KB)'
        }
        df = df.rename(columns=column_mapping)
        
        method_mapping = {
            'barrera': 'barrier',
            'variable': 'cond_var',
            'espera': 'busy_wait'
        }
        df['method'] = df['method'].map(method_mapping)
        
        df.to_csv(csv_out, index=False)
        print("Translation completed.")

    if not os.path.exists(csv_out):
        print(f"Error: {csv_out} not found. Please run the benchmark first.")
        return

    df = pd.read_csv(csv_out)

    df['time(s)'] = pd.to_numeric(df['time(s)'], errors='coerce')
    df['cpu(%)'] = pd.to_numeric(df['cpu(%)'], errors='coerce')
    df['ram(KB)'] = pd.to_numeric(df['ram(KB)'], errors='coerce')

    os.makedirs('plots', exist_ok=True)

    colors = {
        'barrier': '#3B82F6',   # Premium Blue
        'cond_var': '#10B981',  # Emerald Green
        'busy_wait': '#EF4444'  # Vibrant Red
    }
    
    data_sizes = df['data_size'].unique()
    print(f"Available data sizes: {data_sizes}")

    for size in [10000, 1000000]:
        if size not in data_sizes:
            continue
        
        subset = df[df['data_size'] == size]
        if subset.empty:
            continue
            
        plt.figure(figsize=(10, 6))
        for method in subset['method'].unique():
            method_data = subset[subset['method'] == method].sort_values(by='threads')
            color = colors.get(method, '#8B5CF6')
            plt.plot(method_data['threads'], method_data['time(s)'], marker='o', linewidth=2.5, label=method, color=color)
            
        plt.title(f'Execution Time vs Threads (Data Size: {size:,} lines)', fontsize=14, fontweight='bold', pad=15)
        plt.xlabel('Number of Threads', fontsize=12)
        plt.ylabel('Execution Time (seconds)', fontsize=12)
        plt.xscale('log')
        plt.xticks([1, 10, 50, 100], ['1', '10', '50', '100'])
        plt.legend(fontsize=11)
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.tight_layout()
        plt.savefig(f'plots/time_vs_threads_{size}.png', dpi=300)
        plt.close()

    size_for_metrics = 10000
    if size_for_metrics in data_sizes:
        subset = df[df['data_size'] == size_for_metrics]
        
        plt.figure(figsize=(10, 6))
        for method in subset['method'].unique():
            method_data = subset[subset['method'] == method].sort_values(by='threads')
            color = colors.get(method, '#8B5CF6')
            plt.plot(method_data['threads'], method_data['cpu(%)'], marker='s', linewidth=2.5, label=method, color=color)
        plt.title(f'CPU Usage (%) vs Threads (Data Size: {size_for_metrics:,} lines)', fontsize=14, fontweight='bold', pad=15)
        plt.xlabel('Number of Threads', fontsize=12)
        plt.ylabel('CPU Usage (%)', fontsize=12)
        plt.xscale('log')
        plt.xticks([1, 10, 50, 100], ['1', '10', '50', '100'])
        plt.legend(fontsize=11)
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.tight_layout()
        plt.savefig(f'plots/cpu_vs_threads_{size_for_metrics}.png', dpi=300)
        plt.close()

        plt.figure(figsize=(10, 6))
        for method in subset['method'].unique():
            method_data = subset[subset['method'] == method].sort_values(by='threads')
            color = colors.get(method, '#8B5CF6')
            plt.plot(method_data['threads'], method_data['ram(KB)'], marker='^', linewidth=2.5, label=method, color=color)
        plt.title(f'RAM Usage (KB) vs Threads (Data Size: {size_for_metrics:,} lines)', fontsize=14, fontweight='bold', pad=15)
        plt.xlabel('Number of Threads', fontsize=12)
        plt.ylabel('Memory Usage (KB)', fontsize=12)
        plt.xscale('log')
        plt.xticks([1, 10, 50, 100], ['1', '10', '50', '100'])
        plt.legend(fontsize=11)
        plt.grid(True, which="both", ls="--", alpha=0.5)
        plt.tight_layout()
        plt.savefig(f'plots/ram_vs_threads_{size_for_metrics}.png', dpi=300)
        plt.close()

    print("Charts successfully generated and saved in the 'plots' directory.")

if __name__ == "__main__":
    main()
