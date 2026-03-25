#include <cuda_runtime.h>
#include <stdio.h>

// Grid-stride so the reduction occupies only ~50% of the GPU,
// leaving SMs free for concurrent compute kernels on other streams.
__global__ void multiAccumulateKernel(float* dst, float* const* srcs, int numSrcs, int size) {
    int stride = blockDim.x * gridDim.x;
    for (int idx = blockIdx.x * blockDim.x + threadIdx.x; idx < size; idx += stride) {
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
    // Allocate async on the stream so it is ordered with the kernel and free below.
    // cudaFreeAsync requires the pointer to have been allocated with cudaMallocAsync.
    cudaError_t err = cudaMallocAsync(&d_srcs, numSrcs * sizeof(float*), stream);
    if (err != cudaSuccess) { printf("CUDA Error (MallocAsync): %s\n", cudaGetErrorString(err)); return; }

    // Synchronous copy of the tiny pointer array (numSrcs * 8 bytes) — ensures
    // h_srcs remains valid and the copy is done before the kernel starts.
    err = cudaMemcpy(d_srcs, h_srcs, numSrcs * sizeof(float*), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) { printf("CUDA Error (Memcpy): %s\n", cudaGetErrorString(err)); cudaFreeAsync(d_srcs, stream); return; }

    int blockSize = 256;
    int maxBlocks = 512;
    int numBlocks = min(maxBlocks, (size + blockSize - 1) / blockSize);
    multiAccumulateKernel<<<numBlocks, blockSize, 0, stream>>>(dst, d_srcs, numSrcs, size);

    // Free asynchronously after the kernel finishes on this stream — no CPU stall.
    err = cudaFreeAsync(d_srcs, stream);
    if (err != cudaSuccess) printf("CUDA Error (FreeAsync): %s\n", cudaGetErrorString(err));
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
