#ifndef VECTOR_OPS_H
#define VECTOR_OPS_H

#include <chrono>
#include <iostream>


#define VECTOR_SIZE 1000000
/*
==============================================================
Vector Operations List

CPU and OpenCL implementations must support
these operations.

==============================================================
*/


typedef enum
{

    ADD,

    SUBTRACT,

    MULTIPLY,

    DIVIDE,


    SQUARE,

    SQRT,

    EXP,

    LOG,


    RELU,

    SIGMOID,

    TANH,


    SUM_REDUCTION,

    DOT_PRODUCT,


    COPY,

    FILL


} VectorOperation;



/*
Run complete VectorOps test.

This will:

1. Create input data
2. Run CPU
3. Run OpenCL
4. Compare results
5. Print timing

*/

void vector_ops_run();


typedef struct
{

    double total_time_ms;

    double kernel_time_ms;

} OpenCLTiming;

/*
==============================================================
CPU execution
==============================================================
*/

void vector_cpu_execute(
        VectorOperation op,
        float* inputA,
        float* inputB,
        float* output,
        int size);




/*
==============================================================
OpenCL execution
==============================================================
*/

void vector_opencl_execute(
        VectorOperation op,
        float* inputA,
        float* inputB,
        float* output,
        int size,
        OpenCLTiming* timing);




/*
==============================================================
Run benchmark
==============================================================
*/

void vector_ops_run();



#endif