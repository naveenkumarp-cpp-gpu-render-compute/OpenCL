#include "cpu_vector.h"


/*
    Normal CPU vector addition

    CPU executes this loop sequentially
    using available CPU cores/compiler optimization.
*/

void cpuVectorAdd(
        float *A,
        float *B,
        float *C,
        int size)
{
    for(int i=0;i<size;i++)
    {
        C[i] = A[i] + B[i];
    }
}
