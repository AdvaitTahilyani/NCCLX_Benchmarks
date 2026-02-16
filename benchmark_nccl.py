import subprocess
import re
import matplotlib.pyplot as plt
import os
import sys

# Configurations
# Expanded test suite
sizes = [
    1048576,      # 1 MB
    10485760,     # 10 MB
    26214400,     # 25 MB
    52428800,     # 50 MB
    104857600     # 100 MB
]
chares_list = [4, 8, 16, 32, 64]

# Paths
base_dir = os.path.expanduser("~")
prog_direct = os.path.join(base_dir, "NCCLX_Benchmarks", "charm_ncclx_test", "nccl_integration")
prog_single = os.path.join(base_dir, "NCCLX_Benchmarks", "charm_ncclx_single_kernel", "nccl_integration")
prog_spanning = os.path.join(base_dir, "NCCLX_Benchmarks", "charm_ncclx_spanning_tree", "nccl_integration")

# Configuration for spanning tree - test multiple K values
K_values = [2, 4, 8, 16]  # Fan-in factors to test

# Results: {(size, chares): {'direct': time, 'single': time, 'spanning_k2': time, 'spanning_k4': time, ...}}
results = {}

def log(msg):
    print(msg, flush=True)

def run_bench(prog, size, chares, k_val=None):
    in_slurm = "SLURM_JOB_ID" in os.environ
    
    # Determine method name for logging
    if 'test' in prog:
        method_name = 'Direct'
    elif 'spanning_tree' in prog:
        method_name = f'Spanning Tree (K={k_val})'
    else:
        method_name = 'Single'
    
    # Build command
    if k_val is not None:
        base_cmd = f"{prog} {size} {chares} 100 {k_val} +HapiPoolSize 16M"
    else:
        base_cmd = f"{prog} {size} {chares} +HapiPoolSize 16M"
    
    if in_slurm:
        cmd = f"mpiexec -n 1 {base_cmd}"
    else:
        if k_val is not None:
            cmd = f"srun -p h100 -N 1 -n 1 -t 00:20:00 bash -c 'source ~/init.sh; {prog} {size} {chares} 100 {k_val} +HapiPoolSize 16M'"
        else:
            cmd = f"srun -p h100 -N 1 -n 1 -t 00:20:00 bash -c 'source ~/init.sh; {prog} {size} {chares} +HapiPoolSize 16M'"

    log(f"--> Executing: Size={size} Chares={chares} ({method_name})")
    try:
        result = subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=300)
        output = result.stdout + result.stderr
        
        match = re.search(r"AvgTime=(\d+\.\d+) ms", output)
        if match:
            t = float(match.group(1))
            log(f"    Success: {t:.2f} ms")
            return t
        else:
            # Shortened error log
            log(f"    Failed. Snippet: {output[-200:]}")
            return None
    except subprocess.TimeoutExpired:
        log("    Timeout expired!")
        return None
    except Exception as e:
        log(f"    Error: {e}")
        return None

log("Starting Expanded Benchmark Suite...")
if "SLURM_JOB_ID" not in os.environ:
    log("Note: Running via individual srun jobs (slower). Prefer running inside an allocation.")

# Run Loop
for size in sizes:
    for chares in chares_list:
        log(f"\n--- Test Case: {size/1024/1024:.1f} MB / {chares} Chares ---")
        
        # Run baseline methods
        t_direct = run_bench(prog_direct, size, chares)
        t_single = run_bench(prog_single, size, chares)
        
        # Run spanning tree with multiple K values
        spanning_results = {}
        all_spanning_success = True
        for k_val in K_values:
            t_spanning = run_bench(prog_spanning, size, chares, k_val)
            if t_spanning is not None:
                spanning_results[f'spanning_k{k_val}'] = t_spanning
            else:
                all_spanning_success = False
        
        # Store results if baseline methods succeeded and at least one spanning tree worked
        if t_direct is not None and t_single is not None and spanning_results:
            result_entry = {
                'direct': t_direct, 
                'single': t_single
            }
            result_entry.update(spanning_results)
            results[(size, chares)] = result_entry

# Plotting
if not results:
    log("No valid results to plot.")
    sys.exit(1)

log("\nGenerating plot...")
# Filter labels to only those that ran successfully
labels = [f"{s//1024//1024}M/{c}ch" for s in sizes for c in chares_list if (s,c) in results]
direct_times = [results[(s,c)]['direct'] for s in sizes for c in chares_list if (s,c) in results]
single_times = [results[(s,c)]['single'] for s in sizes for c in chares_list if (s,c) in results]

# Collect spanning tree results for each K value
spanning_data = {}
for k_val in K_values:
    key = f'spanning_k{k_val}'
    spanning_data[k_val] = [results[(s,c)].get(key) for s in sizes for c in chares_list if (s,c) in results]

x = range(len(labels))
num_methods = 2 + len(K_values)  # direct, single, + K variants
width = 0.8 / num_methods

fig, ax = plt.subplots(figsize=(20, 8))

# Plot baseline methods
offset = -(num_methods - 1) * width / 2
rects1 = ax.bar([i + offset for i in x], direct_times, width, label='Direct Accumulation')
offset += width
rects2 = ax.bar([i + offset for i in x], single_times, width, label='Single Kernel')
offset += width

# Plot spanning tree variants
spanning_rects = []
for k_val in K_values:
    if spanning_data[k_val] and any(t is not None for t in spanning_data[k_val]):
        # Filter out None values for plotting
        plot_data = [t if t is not None else 0 for t in spanning_data[k_val]]
        rects = ax.bar([i + offset for i in x], plot_data, width, label=f'Spanning Tree (K={k_val})')
        spanning_rects.append(rects)
        offset += width

ax.set_ylabel('Average Time (ms) - Log Scale')
ax.set_title('Charm++ NCCLX Reduction: Comprehensive Comparison')
ax.set_xticks(x)
ax.set_xticklabels(labels, rotation=45, ha='right')
ax.legend(loc='upper left')
ax.set_yscale('log')
ax.grid(True, alpha=0.3, axis='y')

fig.tight_layout()
output_file = os.path.expanduser("~/benchmark_comparison_comprehensive.png")
plt.savefig(output_file, dpi=150)
log(f"Graph saved to {output_file}")

# Also create a summary CSV
csv_file = os.path.expanduser("~/benchmark_results.csv")
with open(csv_file, 'w') as f:
    # Header
    header = "Size(MB),Chares,Direct,Single," + ",".join([f"Spanning_K{k}" for k in K_values])
    f.write(header + "\n")
    
    # Data
    for size in sizes:
        for chares in chares_list:
            if (size, chares) in results:
                row = [f"{size/1024/1024:.1f}", str(chares)]
                row.append(f"{results[(size, chares)]['direct']:.3f}")
                row.append(f"{results[(size, chares)]['single']:.3f}")
                for k_val in K_values:
                    key = f'spanning_k{k_val}'
                    val = results[(size, chares)].get(key)
                    row.append(f"{val:.3f}" if val is not None else "N/A")
                f.write(",".join(row) + "\n")

log(f"CSV results saved to {csv_file}")
