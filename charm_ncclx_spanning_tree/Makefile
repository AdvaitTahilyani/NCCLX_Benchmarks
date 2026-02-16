CHARM_HOME = $(HOME)/charm
CHARMC = $(CHARM_HOME)/bin/charmc

NCCL_HOME = $(HOME)/torchcomms/build/ncclx
CONDA_PREFIX ?= $(HOME)/miniconda3/envs/torchcomms

# Compiler flags
CXXFLAGS = -I$(NCCL_HOME)/include -g -O3
CUFLAGS = -O3 -g

# Linker flags
LDFLAGS = -L$(NCCL_HOME)/lib -L$(CONDA_PREFIX)/lib \
          -lnccl -lstdc++fs -lglog -lunwind -lsodium -lpthread -lfmt \
          -lgflags -ldouble-conversion -ldwarf -lboost_context -lbz2 \
          -Wl,-rpath,$(CONDA_PREFIX)/lib -Wl,-rpath-link,$(CONDA_PREFIX)/lib \
          -L$(CUDA_HOME)/lib64 -lcudart -lmpi -L/opt/apps/gcc13/openmpi/5.0.8/lib

TARGET = nccl_integration
OBJS = nccl_integration.o kernels.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CHARMC) -language charm++ -o $@ $(OBJS) $(LDFLAGS)

nccl_integration.o: nccl_integration.C nccl_integration.decl.h kernels.h
	$(CHARMC) -c nccl_integration.C $(CXXFLAGS)

nccl_integration.decl.h: nccl_integration.ci
	$(CHARMC) nccl_integration.ci

kernels.o: kernels.cu
	nvcc -c kernels.cu -o kernels.o $(CUFLAGS)

kernels.h:
	echo 'extern "C" void allocateDeviceBuffer(float** ptr, int size);' > kernels.h
	echo 'extern "C" void freeDeviceBuffer(float* ptr);' >> kernels.h
	echo 'extern "C" void launchMultiAccumulate(float* dst, float** h_srcs, int numSrcs, int size, cudaStream_t stream);' >> kernels.h
	echo 'extern "C" void launchFill(float* data, float value, int size, cudaStream_t stream);' >> kernels.h
	echo 'extern "C" void launchDummyCompute(float* data, int k, int size, cudaStream_t stream);' >> kernels.h
	echo 'extern "C" void checkCudaErrors();' >> kernels.h

clean:
	rm -f $(TARGET) *.o *.decl.h *.def.h charmrun kernels.h

