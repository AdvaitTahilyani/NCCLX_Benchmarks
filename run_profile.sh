#!/bin/bash
#SBATCH -J nccl_profile
#SBATCH -o profile.%j.out
#SBATCH -e profile.%j.err
#SBATCH -p h100
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -t 0:5:00

# nsys Profiling Job for NCCL Benchmarks
# ----------------------------------------
# Profiles one representative configuration per method:
#   - Direct Accumulation
#   - Single Kernel
#   - Spanning Tree (K=16 by default)
#
# Outputs .nsys-rep files to ~/NCCLX_Benchmarks/profiles/
#
# Override config via environment variables before submitting:
#   PROFILE_SIZE=104857600 PROFILE_CHARES=64 PROFILE_K=16 sbatch run_profile.sh

echo "========================================="
echo "nsys Profiling Suite - NCCL Benchmarks"
echo "========================================="
echo "Start time: $(date)"
echo "Job ID:     $SLURM_JOB_ID"
echo "Node:       $(hostname)"
echo ""

# Load base environment (provides correct gcc/cuda/openmpi the apps were compiled with)
source ~/init.sh

# Use nsys via full path - do NOT `module load nvidia/26.1` as it swaps
# cuda/12.8->13.1 and openmpi/5.0.8->5.0.9, breaking the compiled binaries.
NSYS=/home1/apps/nvidia/Linux_x86_64/26.1/compilers/bin/nsys
echo "nsys version: $($NSYS --version 2>&1 | head -1)"
echo ""

# Navigate to benchmark directory
cd ~/NCCLX_Benchmarks

# Config (override via env vars if desired)
SIZE=${PROFILE_SIZE:-104857600}    # 100 MB
CHARES=${PROFILE_CHARES:-64}
K=${PROFILE_K:-"4 8 16"}             # space-separated list of K values
KCOMP=${PROFILE_KCOMP:-500}

echo "Profiling config:"
echo "  SIZE=$SIZE  CHARES=$CHARES  K=$K  KCOMP=$KCOMP"
echo ""

python3 -u profile_nccl.py \
    --size   "$SIZE"        \
    --chares "$CHARES"      \
    --k      $K             \
    --kcomp  "$KCOMP"

echo ""
echo "========================================="
echo "Profiling Complete!"
echo "End time: $(date)"
echo "========================================="
echo ""
echo "Profile files in ~/NCCLX_Benchmarks/profiles/"
echo "Copy .nsys-rep to your local machine and open with Nsight Systems GUI."
