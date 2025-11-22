#!/bin/bash
#SBATCH -J charm_nccl_test
#SBATCH -o charm_nccl_test.out
#SBATCH -e charm_nccl_test.err
#SBATCH -p h100
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -t 00:10:00

# Run script for Charm++ NCCLX integration

# Load necessary modules
module unload impi 2>/dev/null || true
module load openmpi/5.0.8
module load cuda/12.8
module load ninja/1.13.1

# Set up paths
export CHARM_HOME=~/charm
export PATH=$CHARM_HOME/bin:$PATH
export NCCL_HOME=~/torchcomms/build/ncclx
export LD_LIBRARY_PATH=$NCCL_HOME/lib:~/miniconda3/envs/torchcomms/lib:$LD_LIBRARY_PATH

# Ensure we are in the right directory
cd ~/charm_ncclx_single_kernel

# Run with 1 process (node) and 4 chares (simulating 1 GPU per node)
echo "Running Charm++ NCCLX Integration Test..."
echo "Job ID: $SLURM_JOB_ID"

if [ -f ./charmrun ]; then
    ./charmrun +p1 ./nccl_integration 1048576 4 +HapiPoolSize 16M
else
    mpiexec -n 1 ./nccl_integration 1048576 4 +HapiPoolSize 16M
fi
