#!/bin/bash
#SBATCH -J nccl_comprehensive_benchmark
#SBATCH -o benchmark_comprehensive.%j.out
#SBATCH -e benchmark_comprehensive.%j.err
#SBATCH -p h100
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -t 1:00:00

# Comprehensive NCCL Benchmark Suite
# Tests Direct, Single Kernel, and Spanning Tree with K={2,4,8,16}

echo "========================================="
echo "NCCL Comprehensive Benchmark Suite"
echo "========================================="
echo "Start time: $(date)"
echo "Job ID: $SLURM_JOB_ID"
echo "Node: $(hostname)"
echo ""

# Load environment
source ~/init.sh

# Navigate to benchmark directory
cd ~/NCCLX_Benchmarks

# Run the comprehensive benchmark
python3 -u benchmark_nccl.py

echo ""
echo "========================================="
echo "Benchmark Complete!"
echo "End time: $(date)"
echo "========================================="
echo ""
echo "Results saved to:"
echo "  - ~/benchmark_comparison_comprehensive.png"
echo "  - ~/benchmark_results.csv"
