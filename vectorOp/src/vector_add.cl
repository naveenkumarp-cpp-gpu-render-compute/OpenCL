/*
    OpenCL Kernel

    Every GPU work item handles one element:

        C[i] = A[i] + B[i]

    Example:

        Work item 0 -> C[0]
        Work item 1 -> C[1]
        Work item 2 -> C[2]                                                                

*/

__kernel void vector_add(
        __global float *A,
        __global float *B,
        __global float *C)
{
    int id = get_global_id(0);

    C[id] = A[id] + B[id];
}
