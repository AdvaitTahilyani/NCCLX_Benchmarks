extern "C" void allocateDeviceBuffer(float** ptr, int size);
extern "C" void freeDeviceBuffer(float* ptr);
extern "C" void launchAccumulate(float* dst, const float* src, int size, cudaStream_t stream);
extern "C" void launchFill(float* data, float value, int size, cudaStream_t stream);
extern "C" void checkCudaErrors();
