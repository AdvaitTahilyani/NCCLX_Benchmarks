#include <cuda_runtime.h>
#include <stdio.h>

// Kernel to accumulate data: dst[i] += src[i]
__global__ void accumulateKernel(float* dst, const float* src, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
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
    int numBlocks = (size + blockSize - 1) / blockSize;
    accumulateKernel<<<numBlocks, blockSize, 0, stream>>>(dst, src, size);
}

extern "C" void checkCudaErrors() {
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA Error: %s\n", cudaGetErrorString(err));
    }
}


