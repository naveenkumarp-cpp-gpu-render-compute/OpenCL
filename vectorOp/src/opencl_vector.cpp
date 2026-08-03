#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "opencl_vector.h"

/*
    Utility: Load kernel source from file
*/
char* loadKernel(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("Cannot open kernel: %s\n", filename);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char* source = (char*)malloc(size + 1);
    fread(source, 1, size, fp);
    source[size] = '\0';
    fclose(fp);
    return source;
}

/*
    Vector addition using OpenCL
    C = A + B
*/
void openclVectorAdd(float* A, float* B, float* C, int size) {
    // 1. Get platform and GPU device
    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // 2. Create context and command queue
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    // 3. Create memory buffers on GPU
    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(float) * size, A, NULL);
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                 sizeof(float) * size, B, NULL);
    cl_mem bufC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                 sizeof(float) * size, NULL, NULL);

    // 4. Load and build kernel program
    char* source = loadKernel("vectorOp/src/vector_add.cl");
    cl_program program = clCreateProgramWithSource(context, 1,
                                                   (const char**)&source, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "vector_add", NULL);

    // 5. Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);

    // 6. Execute kernel
    size_t globalSize = size;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

    // 7. Read results back to CPU
    clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0,
                        sizeof(float) * size, C, 0, NULL, NULL);

    // 8. wait for results to finish
    clFinish(queue);

    // 9. Cleanup
    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    free(source);
}
