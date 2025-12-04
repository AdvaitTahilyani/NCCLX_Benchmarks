extern "C" void allocateDeviceBuffer(float** ptr, int size);
extern "C" void freeDeviceBuffer(float* ptr);
extern "C" void launchMultiAccumulate(float* dst, float** h_srcs, int numSrcs, int size, cudaStream_t stream);
extern "C" void launchFill(float* data, float value, int size, cudaStream_t stream);
extern "C" void launchDummyCompute(float* data, int k, int size, cudaStream_t stream);
extern "C" void checkCudaErrors();
