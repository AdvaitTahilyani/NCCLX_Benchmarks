#include "nccl_integration.decl.h"
#include <nccl.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

extern "C" void allocateDeviceBuffer(float** ptr, int size);
extern "C" void freeDeviceBuffer(float* ptr);
extern "C" void launchAccumulate(float* dst, const float* src, int size, cudaStream_t stream);
extern "C" void launchFill(float* data, float value, int size, cudaStream_t stream);
extern "C" void launchDummyCompute(float* data, int k, int size, cudaStream_t stream);
extern "C" void checkCudaErrors();

CProxy_Main mainProxy;
CProxy_NodeManager nodeManagerProxy;
CProxy_Worker workerProxy;
int vectorSize = 1024 * 1024;
int numCharesPerNode = 4;
int kIterations = 100;

#define NCCLCHECK(cmd) do {                         \
  ncclResult_t r = cmd;                             \
  if (r!= ncclSuccess) {                            \
    printf("Failed, NCCL error %s:%d '%s'\n",       \
        __FILE__,__LINE__,ncclGetErrorString(r));   \
    CkExit(1);                                      \
  }                                                 \
} while(0)

class Main : public CBase_Main {
    int numNodesReady;
    int nodesFinishedStep;
    int currentIter;
    int totalIters;
    int warmupIters;
    double startTime;

public:
    Main(CkArgMsg* m) : numNodesReady(0), nodesFinishedStep(0), currentIter(0), totalIters(10), warmupIters(2) {
        if (m->argc > 1) vectorSize = atoi(m->argv[1]);
        if (m->argc > 2) numCharesPerNode = atoi(m->argv[2]);
        if (m->argc > 3) kIterations = atoi(m->argv[3]);
        delete m;

        mainProxy = thisProxy;
        ncclUniqueId id;
        NCCLCHECK(ncclGetUniqueId(&id));
        nodeManagerProxy = CProxy_NodeManager::ckNew(sizeof(ncclUniqueId), (char*)&id);
    }

    void initDone() {
        numNodesReady++;
        if (numNodesReady == CkNumPes()) {
            workerProxy = CProxy_Worker::ckNew(CkNumPes() * numCharesPerNode);
            // Allow workers to init, then start
            CkStartQD(CkCallback(CkIndex_Worker::startStep(), workerProxy));
        }
    }

    void nodeDone() {
        nodesFinishedStep++;
        if (nodesFinishedStep == CkNumPes()) {
            nodesFinishedStep = 0;
            currentIter++;
            
            if (currentIter == warmupIters) {
                printf("Warmup complete. Starting measurement.\n");
                startTime = CmiWallTimer();
            }

            if (currentIter < totalIters + warmupIters) {
                 workerProxy.startStep();
            } else {
                double endTime = CmiWallTimer();
                double avgTime = (endTime - startTime) / totalIters;
                printf("BENCHMARK_RESULT: Method=Direct Accumulation, Size=%d, Chares=%d, AvgTime=%f ms\n", 
                       vectorSize, numCharesPerNode, avgTime * 1000.0);
                CkExit();
            }
        }
    }
};

class NodeManager : public CBase_NodeManager {
    ncclComm_t comm;
    cudaStream_t stream;
    float* d_resultBuffer;
    int contributions;

public:
    NodeManager(int nBytes, char* idBytes) {
        contributions = 0;
        ncclUniqueId id;
        memcpy(&id, idBytes, sizeof(ncclUniqueId));

        cudaSetDevice(0); 
        NCCLCHECK(ncclCommInitRank(&comm, CkNumPes(), id, CkMyPe()));
        cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking);
        allocateDeviceBuffer(&d_resultBuffer, vectorSize);

        mainProxy.initDone();
    }

    void registerWorker() {}

    void depositData(float* d_data, int size, cudaEvent_t readyEvent) {
        cudaStreamWaitEvent(stream, readyEvent, 0);
        launchAccumulate(d_resultBuffer, d_data, size, stream);
        contributions++;
        checkCompletion();
    }

    void checkCompletion() {
        if (contributions == numCharesPerNode) {
            NCCLCHECK(ncclAllReduce(d_resultBuffer, d_resultBuffer, vectorSize, ncclFloat, ncclSum, comm, stream));
            cudaStreamSynchronize(stream);
            
            // Reset for next iteration
            contributions = 0;
            // Signal completion
            mainProxy.nodeDone();
        }
    }
};

class Worker : public CBase_Worker {
    float* d_data;
    cudaStream_t computeStream;
    cudaEvent_t readyEvent;
public:
    Worker() {
        allocateDeviceBuffer(&d_data, vectorSize);
        cudaStreamCreateWithFlags(&computeStream, cudaStreamNonBlocking);
        cudaEventCreate(&readyEvent);
        launchFill(d_data, 1.0f, vectorSize, computeStream);
    }

    void startStep() {
        launchDummyCompute(d_data, kIterations, vectorSize, computeStream);
        cudaEventRecord(readyEvent, computeStream);
        NodeManager* myNode = nodeManagerProxy.ckLocalBranch();
        myNode->depositData(d_data, vectorSize, readyEvent);
    }
};

#include "nccl_integration.def.h"
