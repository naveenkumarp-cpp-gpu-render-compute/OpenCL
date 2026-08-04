#include "vectorOp.h"

#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>



/*
==============================================================================
Timer
==============================================================================
*/

static double get_time_ms()
{
    auto now =
        std::chrono::high_resolution_clock::now();


    static auto start =
        now;


    return std::chrono::duration<double,std::milli>(
            now-start).count();
}








void vector_opencl_execute(
        VectorOperation op,
        float* inputA,
        float* inputB,
        float* output,
        int size,
        OpenCLTiming* timing);





/*
==============================================================================
Operation name
==============================================================================
*/

static const char* get_operation_name(
        VectorOperation op)
{

    switch(op)
    {

    case ADD:
        return "ADD";


    case SUBTRACT:
        return "SUBTRACT";


    case MULTIPLY:
        return "MULTIPLY";


    case DIVIDE:
        return "DIVIDE";



    case SQUARE:
        return "SQUARE";


    case SQRT:
        return "SQRT";


    case EXP:
        return "EXP";


    case LOG:
        return "LOG";



    case RELU:
        return "RELU";


    case SIGMOID:
        return "SIGMOID";


    case TANH:
        return "TANH";



    case SUM_REDUCTION:
        return "SUM_REDUCTION";


    case DOT_PRODUCT:
        return "DOT_PRODUCT";



    case COPY:
        return "COPY";


    case FILL:
        return "FILL";


    default:

        return "UNKNOWN";

    }

}







/*
==============================================================================
Compare
==============================================================================
*/

static int compare_results(
        VectorOperation op,
        float* cpu,
        float* gpu,
        int size,
        float* max_error)
{

    int count = size;


    /*
    Reduction operations generate one output
    */

    if(op == SUM_REDUCTION ||
       op == DOT_PRODUCT)
    {
        count = 1;
    }



    float error = 0.0f;



    for(int i=0;i<count;i++)
    {

        float diff =
            fabs(cpu[i]-gpu[i]);


        if(diff > error)
            error = diff;

    }



    *max_error = error;



    /*
    Relative tolerance for floating point math
    */

    float tolerance = 0.001f;



    if(op == EXP ||
       op == LOG ||
       op == TANH ||
       op == SIGMOID)
    {
        tolerance = 0.01f;
    }



    return error <= tolerance;
}





/*
==============================================================================
Vector benchmark
==============================================================================
*/


void vector_ops_run()
{

    int size = VECTOR_SIZE;



    float* A =
        (float*)malloc(sizeof(float)*size);


    float* B =
        (float*)malloc(sizeof(float)*size);


    float* cpu_result =
        (float*)malloc(sizeof(float)*size);


    float* gpu_result =
        (float*)malloc(sizeof(float)*size);




   for(int i=0;i<size;i++)
{
    A[i] = (i % 100) * 0.01f;
    B[i] = 2.0f;
}





    VectorOperation tests[] =
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
};




    int count =
        sizeof(tests)/sizeof(tests[0]);





    for(int i=0;i<count;i++)
    {

        VectorOperation op =
            tests[i];



        /*
        CPU
        */

        double cpu_start =
            get_time_ms();



        vector_cpu_execute(
                op,
                A,
                B,
                cpu_result,
                size);



        double cpu_time =
            get_time_ms()
            -
            cpu_start;



        





        /*
        OPENCL
        */


        OpenCLTiming gpu_time;



        vector_opencl_execute(
                op,
                A,
                B,
                gpu_result,
                size,
                &gpu_time);

        printf("\nCPU ");

        printf("Time : %.6f ms\n",
                cpu_time);


        printf("OPENCL\n");


        printf("Total Time  : %.6f ms\n",
                gpu_time.total_time_ms);


        printf("Kernel Time : %.6f ms\n",
                gpu_time.kernel_time_ms);





        /*
        Compare
        */


        float max_error;


        int pass =
compare_results(
        op,
        cpu_result,
        gpu_result,
        size,
        &max_error);




        printf("Validation: %s\n",
                pass ? "PASS":"FAIL");


    }




    free(A);

    free(B);

    free(cpu_result);

    free(gpu_result);

}
