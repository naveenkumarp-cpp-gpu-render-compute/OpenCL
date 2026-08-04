/*
==============================================================================
Vector Operations OpenCL Kernels

Supported:

ADD
SUBTRACT
MULTIPLY
DIVIDE

SQUARE
SQRT
EXP
LOG

RELU
SIGMOID
TANH

SUM_REDUCTION
DOT_PRODUCT

COPY
FILL

==============================================================================
*/


//==============================================================================
// Basic arithmetic
//==============================================================================


__kernel void vector_add_kernel(
        __global float* A,
        __global float* B,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = A[id] + B[id];
}



__kernel void vector_subtract_kernel(
        __global float* A,
        __global float* B,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = A[id] - B[id];
}



__kernel void vector_multiply_kernel(
        __global float* A,
        __global float* B,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = A[id] * B[id];
}



__kernel void vector_divide_kernel(
        __global float* A,
        __global float* B,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = A[id] / B[id];
}




//==============================================================================
// Mathematical operations
//==============================================================================


__kernel void vector_square_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = A[id] * A[id];
}



__kernel void vector_sqrt_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = sqrt(A[id]);
}



__kernel void vector_exp_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = exp(A[id]);
}



__kernel void vector_log_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = log(A[id]);
}




//==============================================================================
// Activation functions
//==============================================================================


__kernel void vector_relu_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = A[id] > 0 ? A[id] : 0.0f;
}



__kernel void vector_sigmoid_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = 1.0f /
            (1.0f + exp(-A[id]));
}



__kernel void vector_tanh_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = tanh(A[id]);
}




//==============================================================================
// Reduction operations
//==============================================================================


__kernel void vector_sum_reduction_kernel(
        __global float* A,
        __global float* C,
        int size)
{

    float sum = 0.0f;


    for(int i=0;i<size;i++)
    {
        sum += A[i];
    }


    C[0] = sum;
}




__kernel void vector_dot_product_kernel(
        __global float* A,
        __global float* B,
        __global float* C,
        int size)
{

    float result = 0.0f;


    for(int i=0;i<size;i++)
    {
        result += A[i] * B[i];
    }


    C[0] = result;
}




//==============================================================================
// Memory operations
//==============================================================================


__kernel void vector_copy_kernel(
        __global float* A,
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = A[id];
}



__kernel void vector_fill_kernel(
        __global float* C)
{
    int id = get_global_id(0);

    C[id] = 1.0f;
}
