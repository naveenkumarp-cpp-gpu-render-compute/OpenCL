#include "vectorOp.h"

#include <math.h>


/*
==============================================================================
CPU Vector Operations

Reference implementation.

Purpose:

    - Correctness reference for OpenCL
    - Easy debugging
    - Simple readable implementation

Notes:

    Element-wise operations:
        C[i] = operation(A[i], B[i])

    Reduction operations:
        Result stored in C[0]

==============================================================================
*/


void vector_cpu_execute(
        VectorOperation op,
        float* A,
        float* B,
        float* C,
        int size)
{

    int i;


    switch(op)
    {


    //======================================================================
    // Basic arithmetic
    //======================================================================


    case ADD:

        for(i=0;i<size;i++)
            C[i]=A[i]+B[i];

        break;



    case SUBTRACT:

        for(i=0;i<size;i++)
            C[i]=A[i]-B[i];

        break;



    case MULTIPLY:

        for(i=0;i<size;i++)
            C[i]=A[i]*B[i];

        break;



    case DIVIDE:

        for(i=0;i<size;i++)
        {
            if(B[i]!=0.0f)
                C[i]=A[i]/B[i];
            else
                C[i]=0.0f;
        }

        break;



    //======================================================================
    // Mathematical operations
    //======================================================================


    case SQUARE:

        for(i=0;i<size;i++)
            C[i]=A[i]*A[i];

        break;



    case SQRT:

        for(i=0;i<size;i++)
            C[i]=sqrtf(A[i]);

        break;



    case EXP:

        for(i=0;i<size;i++)
            C[i]=expf(A[i]);

        break;



    case LOG:

        for(i=0;i<size;i++)
            C[i]=logf(A[i]);

        break;



    //======================================================================
    // Activation functions
    //======================================================================


    case RELU:

        for(i=0;i<size;i++)
            C[i]=(A[i]>0.0f)?A[i]:0.0f;

        break;



    case SIGMOID:

        for(i=0;i<size;i++)
            C[i]=1.0f/(1.0f+expf(-A[i]));

        break;



    case TANH:

        for(i=0;i<size;i++)
            C[i]=tanhf(A[i]);

        break;



    //======================================================================
    // Reduction operations
    //======================================================================


    case SUM_REDUCTION:

        C[0]=0.0f;

        for(i=0;i<size;i++)
            C[0]+=A[i];

        break;



    case DOT_PRODUCT:

        C[0]=0.0f;

        for(i=0;i<size;i++)
            C[0]+=A[i]*B[i];

        break;



    //======================================================================
    // Memory operations
    //======================================================================


    case COPY:

        for(i=0;i<size;i++)
            C[i]=A[i];

        break;



    case FILL:

        for(i=0;i<size;i++)
            C[i]=A[0];

        break;



    default:

        break;

    }

}
