#include <cuda_runtime.h>
#include <stdio.h>

__global__ void multiAccumulateKernel(float* dst, float* const* srcs, int numSrcs, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        float sum = 0.0f;
        for (int k = 0; k < numSrcs; k++) {
             sum += srcs[k][idx];
        }
        dst[idx] = sum;
    }
}

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

extern "C" void launchMultiAccumulate(float* dst, float** h_srcs, int numSrcs, int size, cudaStream_t stream) {
    float** d_srcs;
    // Sync allocation
    cudaError_t err = cudaMalloc(&d_srcs, numSrcs * sizeof(float*));
    if (err != cudaSuccess) { printf("CUDA Error (Malloc): %s\n", cudaGetErrorString(err)); return; }
    
    // Sync copy (copying to device)
    err = cudaMemcpy(d_srcs, h_srcs, numSrcs * sizeof(float*), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) { printf("CUDA Error (Memcpy): %s\n", cudaGetErrorString(err)); return; }

    int blockSize = 256;
    int numBlocks = (size + blockSize - 1) / blockSize;
    multiAccumulateKernel<<<numBlocks, blockSize, 0, stream>>>(dst, d_srcs, numSrcs, size);
    
    // Sync before free to ensure kernel is done using d_srcs
    // This kills overlap/performance but ensures correctness for debugging
    cudaStreamSynchronize(stream);
    
    err = cudaFree(d_srcs);
    if (err != cudaSuccess) printf("CUDA Error (Free): %s\n", cudaGetErrorString(err));
}

extern "C" void checkCudaErrors() {
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        printf("CUDA Error: %s\n", cudaGetErrorString(err));
    }
}

__global__ void dummyComputeKernel(float* data, int k, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        float val = data[idx];
        for (int i = 0; i < k; i++) {
            val = val * val + 0.0001f;
        }
        data[idx] = val;
    }
}

extern "C" void launchDummyCompute(float* data, int k, int size, cudaStream_t stream) {
    int blockSize = 256;
    int numBlocks = (size + blockSize - 1) / blockSize;
    dummyComputeKernel<<<numBlocks, blockSize, 0, stream>>>(data, k, size);
}
