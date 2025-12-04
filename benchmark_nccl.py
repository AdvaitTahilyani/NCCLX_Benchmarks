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

# Results: {(size, chares): {'direct': time, 'single': time}}
results = {}

def log(msg):
    print(msg, flush=True)

def run_bench(prog, size, chares):
    in_slurm = "SLURM_JOB_ID" in os.environ
    
    # Using mpiexec for efficiency if in SLURM
    base_cmd = f"{prog} {size} {chares} +HapiPoolSize 16M"
    
    if in_slurm:
        cmd = f"mpiexec -n 1 {base_cmd}"
    else:
        cmd = f"srun -p h100 -N 1 -n 1 -t 00:20:00 bash -c 'source ~/init.sh; {prog} {size} {chares} +HapiPoolSize 16M'"

    log(f"--> Executing: Size={size} Chares={chares} ({'Direct' if 'test' in prog else 'Single'})")
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
        
        t_direct = run_bench(prog_direct, size, chares)
        t_single = run_bench(prog_single, size, chares)
        
        if t_direct is not None and t_single is not None:
            results[(size, chares)] = {'direct': t_direct, 'single': t_single}

# Plotting
if not results:
    log("No valid results to plot.")
    sys.exit(1)

log("\nGenerating plot...")
# Filter labels to only those that ran successfully
labels = [f"{s//1024//1024}M/{c}ch" for s in sizes for c in chares_list if (s,c) in results]
direct_times = [results[(s,c)]['direct'] for s in sizes for c in chares_list if (s,c) in results]
single_times = [results[(s,c)]['single'] for s in sizes for c in chares_list if (s,c) in results]

x = range(len(labels))
width = 0.35

fig, ax = plt.subplots(figsize=(14, 7))
rects1 = ax.bar([i - width/2 for i in x], direct_times, width, label='Direct Accumulation')
rects2 = ax.bar([i + width/2 for i in x], single_times, width, label='Single Kernel')

ax.set_ylabel('Average Time (ms) - Log Scale')
ax.set_title('Charm++ NCCLX Reduction: Direct vs Single Kernel')
ax.set_xticks(x)
ax.set_xticklabels(labels, rotation=45, ha='right')
ax.legend()
ax.set_yscale('log')

# Add labels (sparse if too many bars)
if len(x) < 20:
    ax.bar_label(rects1, padding=3, fmt='%.1f')
    ax.bar_label(rects2, padding=3, fmt='%.1f')

fig.tight_layout()
output_file = os.path.expanduser("~/benchmark_comparison.png")
plt.savefig(output_file)
log(f"Graph saved to {output_file}")
