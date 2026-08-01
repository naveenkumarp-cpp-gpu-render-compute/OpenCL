#include <iostream>
#include <chrono>

#include "vectorOp.h"

#include "cpu_vector.h"
#include "opencl_vector.h"

#define N 1000000

void vectorTest()
{
    std::cout << "\nVector Addition Test\n";
    std::cout << "--------------------\n";

    float *A = new float[N];
    float *B = new float[N];

    float *cpuResult = new float[N];
    float *gpuResult = new float[N];

    /*
        Initialize input data
    */

    for (int i = 0; i < N; i++)
    {
        A[i] = i;
        B[i] = 2 * i;
    }

    /*
        CPU execution timing
    */

    auto cpuStart =
        std::chrono::high_resolution_clock::now();

    cpuVectorAdd(
        A,
        B,
        cpuResult,
        N);

    auto cpuEnd =
        std::chrono::high_resolution_clock::now();

    double cpuTime =
        std::chrono::duration<double>(
            cpuEnd - cpuStart)
            .count();

    std::cout
        << "CPU time    : "
        << cpuTime
        << " sec\n";

    /*
        OpenCL execution timing

        Includes:
        - OpenCL setup
        - memory transfer
        - kernel execution
        - result copy back

    */

    auto gpuStart =
        std::chrono::high_resolution_clock::now();

    openclVectorAdd(
        A,
        B,
        gpuResult,
        N);

    auto gpuEnd =
        std::chrono::high_resolution_clock::now();

    double gpuTime =
        std::chrono::duration<double>(
            gpuEnd - gpuStart)
            .count();

    std::cout
        << "OpenCL time : "
        << gpuTime
        << " sec\n";

    /*
        Verify CPU and GPU results
    */

    bool match = true;

    for (int i = 0; i < N; i  = i + 4)
    {
        if (cpuResult[i] != gpuResult[i])
        {
            match = false;
            break;
        }
    }

    if (match)
        std::cout
            << "Vector result MATCH\n";
    else
        std::cout
            << "Vector result FAILED\n";

    /*
        Cleanup host memory
    */

    delete[] A;
    delete[] B;

    delete[] cpuResult;
    delete[] gpuResult;
}
