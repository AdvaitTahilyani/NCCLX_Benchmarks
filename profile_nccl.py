"""
nsys Profiling Script for NCCL Benchmarks
==========================================
Profiles a targeted subset of configurations (not all combos - that would
generate gigabytes of data). Generates one .nsys-rep file per method per case.

Usage (inside an allocation, after sourcing init.sh and loading nvidia module):
    python3 profile_nccl.py [--size SIZE] [--chares CHARES] [--k K [K ...]] [--warmup WARMUP] [--iters ITERS]

    --k accepts one or more values, e.g.:  --k 4 16

Outputs:
    profiles/<method>_<size>mb_<chares>ch.nsys-rep   (viewable in Nsight GUI)
    profiles/<method>_<size>mb_<chares>ch.stats.txt  (text summary)
"""

import subprocess
import os
import sys
import argparse

# Use full path to nsys to avoid loading the nvidia module which would swap
# cuda/openmpi versions and break the compiled binaries.
NSYS = os.environ.get(
    "NSYS_BIN",
    "/home1/apps/nvidia/Linux_x86_64/26.1/compilers/bin/nsys"
)
assert os.path.isfile(NSYS), f"nsys not found at {NSYS}. Set NSYS_BIN env var."

# ---------------------------------------------------------------------------
# Argument parsing
# ---------------------------------------------------------------------------
parser = argparse.ArgumentParser(description="nsys profiling for NCCL benchmarks")
parser.add_argument("--size",   type=int,   default=104857600,  help="Vector size in floats (default: 100MB = 104857600)")
parser.add_argument("--chares", type=int,   default=64,         help="Number of chares (default: 64)")
parser.add_argument("--k",      type=int,   nargs='+', default=[4, 16], help="Spanning tree K values to profile (default: 4 16)")
parser.add_argument("--warmup", type=int,   default=2,          help="Warmup iterations inside the app (default: 2)")
parser.add_argument("--iters",  type=int,   default=5,          help="Timed iterations inside the app (default: 5)")
parser.add_argument("--kcomp",  type=int,   default=500,       help="Dummy compute iterations k (default: 1000)")
args = parser.parse_args()

size     = args.size
chares   = args.chares
k_values = args.k   # list of K values to profile
kcomp    = args.kcomp

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
base_dir     = os.path.expanduser("~")
prog_direct  = os.path.join(base_dir, "NCCLX_Benchmarks", "charm_ncclx_test",         "nccl_integration")
prog_single  = os.path.join(base_dir, "NCCLX_Benchmarks", "charm_ncclx_single_kernel", "nccl_integration")
prog_spanning = os.path.join(base_dir, "NCCLX_Benchmarks", "charm_ncclx_spanning_tree", "nccl_integration")

profile_dir = os.path.join(base_dir, "NCCLX_Benchmarks", "profiles")
os.makedirs(profile_dir, exist_ok=True)

size_mb_str = f"{size // (1024*1024)}mb"

# ---------------------------------------------------------------------------
# Verify programs exist
# ---------------------------------------------------------------------------
for name, prog in [("Direct", prog_direct), ("Single", prog_single), ("Spanning", prog_spanning)]:
    assert os.path.isfile(prog), f"Binary not found for {name}: {prog}"

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def log(msg):
    print(msg, flush=True)

def run_profile(label, app_cmd, out_stem):
    """
    Wrap `app_cmd` with nsys profile and write .nsys-rep + stats.
    `out_stem` is the full path prefix (without extension).
    """
    in_slurm = "SLURM_JOB_ID" in os.environ
    assert in_slurm, (
        "nsys profiling should be run inside a Slurm allocation "
        "(mpiexec is needed). Submit via run_profile.sh."
    )

    rep_file   = out_stem + ".nsys-rep"
    stats_file = out_stem + ".stats.txt"

    nsys_cmd = (
        f"mpiexec -n 1 "
        f"{NSYS} profile "
        f"--trace=cuda,nvtx "            # CUDA ops + NVTX markers
        f"--cuda-memory-usage=true "     # show memory allocs in timeline
        f"--force-overwrite=true "
        f"--output={out_stem} "
        f"{app_cmd}"
    )

    log(f"\n{'='*60}")
    log(f"  Profiling: {label}")
    log(f"  Output:    {rep_file}")
    log(f"{'='*60}")
    log(f"  CMD: {nsys_cmd}\n")

    try:
        result = subprocess.run(
            nsys_cmd, shell=True, text=True,
            timeout=600  # 10 min max per profile run
        )
        if result.returncode != 0:
            log(f"  [WARNING] nsys returned exit code {result.returncode} for {label}")
        else:
            log(f"  [OK] Profile written to {rep_file}")
            # Generate stats from the .nsys-rep file
            stats_cmd = f"{NSYS} stats --force-export=true {rep_file} > {stats_file} 2>&1"
            subprocess.run(stats_cmd, shell=True, timeout=120)
            log(f"  [OK] Stats written to {stats_file}")
    except subprocess.TimeoutExpired:
        log(f"  [ERROR] Timeout profiling {label}")

# ---------------------------------------------------------------------------
# Build the app commands for each method
# ---------------------------------------------------------------------------
hapi_flag = "+HapiPoolSize 16M"

cases = [
    (
        f"Direct Accumulation ({size_mb_str}, {chares} chares)",
        f"{prog_direct} {size} {chares} {kcomp} {hapi_flag}",
        os.path.join(profile_dir, f"direct_{size_mb_str}_{chares}ch"),
    ),
    (
        f"Single Kernel ({size_mb_str}, {chares} chares)",
        f"{prog_single} {size} {chares} {kcomp} {hapi_flag}",
        os.path.join(profile_dir, f"single_{size_mb_str}_{chares}ch"),
    ),
]

# Add one spanning tree case per K value
for k_span in k_values:
    cases.append((
        f"Spanning Tree K={k_span} ({size_mb_str}, {chares} chares)",
        f"{prog_spanning} {size} {chares} {kcomp} {k_span} {hapi_flag}",
        os.path.join(profile_dir, f"spanning_k{k_span}_{size_mb_str}_{chares}ch"),
    ))

# ---------------------------------------------------------------------------
# Run profiles
# ---------------------------------------------------------------------------
log(f"nsys Profiling Suite")
log(f"  Config: size={size} ({size_mb_str}), chares={chares}, K={k_values}, kcomp={kcomp}")
log(f"  Output dir: {profile_dir}")
log(f"  Nsight GUI: open .nsys-rep files with `nsys-ui`\n")

for label, app_cmd, out_stem in cases:
    run_profile(label, app_cmd, out_stem)

log(f"\n{'='*60}")
log("All profiles complete!")
log(f"Files in {profile_dir}:")
for f in sorted(os.listdir(profile_dir)):
    fpath = os.path.join(profile_dir, f)
    size_kb = os.path.getsize(fpath) // 1024
    log(f"  {f:60s}  {size_kb:>8} KB")
log(f"{'='*60}")
log("To view profiles: copy .nsys-rep files locally and open with Nsight Systems GUI.")
log("Or run:  nsys stats <file>.nsys-rep  for a text summary.")
