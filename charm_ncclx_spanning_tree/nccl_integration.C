#include "nccl_integration.decl.h"
#include <nccl.h>
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>

extern "C" void allocateDeviceBuffer(float** ptr, int size);
extern "C" void freeDeviceBuffer(float* ptr);
extern "C" void launchMultiAccumulate(float* dst, float** h_srcs, int numSrcs, int size, cudaStream_t stream);
extern "C" void launchFill(float* data, float value, int size, cudaStream_t stream);
extern "C" void launchDummyCompute(float* data, int k, int size, cudaStream_t stream);
extern "C" void checkCudaErrors();

CProxy_Main mainProxy;
CProxy_NodeManager nodeManagerProxy;
CProxy_Worker workerProxy;
int vectorSize = 1024 * 1024; 
int numCharesPerNode = 4;
int kIterations = 100;
int K = 4;  // Fan-in factor

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
        if (m->argc > 4) K = atoi(m->argv[4]);
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
                printf("BENCHMARK_RESULT: Method=Spanning Tree K=%d, Size=%d, Chares=%d, AvgTime=%f ms\n", 
                       K, vectorSize, numCharesPerNode, avgTime * 1000.0);
                CkExit();
            }
        }
    }
};

class NodeManager : public CBase_NodeManager {
    ncclComm_t comm;
    cudaStream_t stream;
    float* d_resultBuffer;
    
    // K-grouping state
    std::vector<float*> pendingPtrs;      // Current batch being collected
    std::vector<float*> tempBuffers;      // Pool of allocated temp buffers
    std::vector<float*> currentLevel;     // Results from current reduction level
    int totalReceived;
    int tempBufferIdx;

public:
    NodeManager(int nBytes, char* idBytes) : totalReceived(0), tempBufferIdx(0) {
        ncclUniqueId id;
        memcpy(&id, idBytes, sizeof(ncclUniqueId));

        cudaSetDevice(0); 
        NCCLCHECK(ncclCommInitRank(&comm, CkNumPes(), id, CkMyPe()));
        cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking);
        allocateDeviceBuffer(&d_resultBuffer, vectorSize);

        // Pre-allocate temp buffers for 2-layer approach
        // Level 1: ceil(N/K) buffers for worker groups
        // Level 2: 1 buffer for final accumulation
        int level1Buffers = (numCharesPerNode + K - 1) / K;
        int maxTempBuffers = level1Buffers + 1;
        
        tempBuffers.resize(maxTempBuffers);
        for (int i = 0; i < maxTempBuffers; i++) {
            allocateDeviceBuffer(&tempBuffers[i], vectorSize);
        }
        
        pendingPtrs.reserve(K);
        currentLevel.reserve(maxTempBuffers);
        
        mainProxy.initDone();
    }

    void registerWorker() {}

    void depositData(float* d_data, cudaEvent_t readyEvent) {
        // Wait for worker's compute to finish
        cudaStreamWaitEvent(stream, readyEvent, 0);
        
        pendingPtrs.push_back(d_data);
        totalReceived++;
        
        // Process batch when we have K pointers or all chares have deposited
        if (pendingPtrs.size() == K || totalReceived == numCharesPerNode) {
            float* tempBuf = getNextTempBuffer();
            launchMultiAccumulate(tempBuf, pendingPtrs.data(), 
                                 pendingPtrs.size(), vectorSize, stream);
            currentLevel.push_back(tempBuf);
            pendingPtrs.clear();
        }
        
        // When all chares have deposited, finalize levels
        if (totalReceived == numCharesPerNode) {
            finalizeLevels();
        }
    }

private:
    float* getNextTempBuffer() {
        if (tempBufferIdx >= tempBuffers.size()) {
            printf("ERROR: Temp buffer overflow!\n");
            CkExit(1);
        }
        return tempBuffers[tempBufferIdx++];
    }

    void finalizeLevels() {
        // Two-layer approach:
        // Level 1: Already done in depositData() - workers grouped into ceil(N/K) temp buffers
        // Level 2: Accumulate all temp buffers into final result (single kernel)
        
        float* finalBuffer;
        
        if (currentLevel.size() == 1) {
            // Only one temp buffer, use it directly
            finalBuffer = currentLevel[0];
        } else {
            // Multiple temp buffers, accumulate them all in one kernel
            finalBuffer = getNextTempBuffer();
            launchMultiAccumulate(finalBuffer, currentLevel.data(), 
                                 currentLevel.size(), vectorSize, stream);
        }
        
        // NCCL all-reduce with the final accumulated buffer
        NCCLCHECK(ncclAllReduce(finalBuffer, d_resultBuffer, vectorSize, 
                               ncclFloat, ncclSum, comm, stream));
        cudaStreamSynchronize(stream);
        
        // Reset for next iteration
        pendingPtrs.clear();
        currentLevel.clear();
        totalReceived = 0;
        tempBufferIdx = 0;
        
        mainProxy.nodeDone();
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
        // Record event when compute completes
        cudaEventRecord(readyEvent, computeStream);
        
        // Notify NodeManager (non-blocking on CPU)
        NodeManager* myNode = nodeManagerProxy.ckLocalBranch();
        myNode->depositData(d_data, readyEvent);
    }
};

#include "nccl_integration.def.h"
