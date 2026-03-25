#include <cuda_runtime.h>
#include <stdio.h>

// Kernel to accumulate data: dst[i] += src[i]
// Grid-stride so the reduction occupies only ~50% of the GPU,
// leaving SMs free for concurrent compute kernels on other streams.
__global__ void accumulateKernel(float* dst, const float* src, int size) {
    int stride = blockDim.x * gridDim.x;
    for (int idx = blockIdx.x * blockDim.x + threadIdx.x; idx < size; idx += stride) {
        dst[idx] += src[idx];
    }
}

// Kernel to fill data with a value (for testing)
__global__ void fillKernel(float* data, float value, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        data[idx] = value;
    }
}

extern "C" void allocateDeviceBuffer(float** ptr, int size) {
    cudaMalloc(ptr, size * sizeof(float));
    cudaMemset(*ptr, 0, size * sizeof(float));
}

extern "C" void freeDeviceBuffer(float* ptr) {
    cudaFree(ptr);
}

extern "C" void launchFill(float* data, float value, int size, cudaStream_t stream) {
    int blockSize = 256;
    int numBlocks = (size + blockSize - 1) / blockSize;
    fillKernel<<<numBlocks, blockSize, 0, stream>>>(data, value, size);
}

extern "C" void launchAccumulate(float* dst, const float* src, int size, cudaStream_t stream) {
    int blockSize = 256;
    int maxBlocks = 512;
    int numBlocks = min(maxBlocks, (size + blockSize - 1) / blockSize);
    accumulateKernel<<<numBlocks, blockSize, 0, stream>>>(dst, src, size);
}

extern "C" void checkCudaErrors() {
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA Error: %s\n", cudaGetErrorString(err));
    }
}

// Grid-stride loop: each thread processes multiple elements so the grid is
// small enough to leave SMs free for concurrent reduction kernels.
//
// Uses a stable bounded FMA chain: val = 0.9999*val + 0.0001
// which converges toward 1.0 from any starting point, ensuring ALL k
// iterations do real arithmetic (unlike val*val which overflows to inf
// after ~20 steps and makes remaining iterations trivial).
__global__ void dummyComputeKernel(float* data, int k, int size) {
    int stride = blockDim.x * gridDim.x;
    for (int idx = blockIdx.x * blockDim.x + threadIdx.x; idx < size; idx += stride) {
        float val = data[idx];
        for (int i = 0; i < k; i++) {
            val = fmaf(val, 0.9999f, 0.0001f);
        }
        data[idx] = val;
    }
}

// Cap at 512 blocks (~50% of H100's 132 SMs × 8 blocks/SM = 1056 max),
// leaving headroom for reduction kernels to run concurrently.
extern "C" void launchDummyCompute(float* data, int k, int size, cudaStream_t stream) {
    int blockSize = 256;
    int maxBlocks = 512;
    int numBlocks = min(maxBlocks, (size + blockSize - 1) / blockSize);
    dummyComputeKernel<<<numBlocks, blockSize, 0, stream>>>(data, k, size);
}


