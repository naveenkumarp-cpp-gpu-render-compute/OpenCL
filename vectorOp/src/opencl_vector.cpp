#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#include "opencl_vector.h"


/*
    Load .cl kernel file
*/
char* loadKernel(const char* filename)
{
    FILE* fp = fopen(filename, "rb");

    if(!fp)
    {
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
    Print available OpenCL hardware

    Platform
        |
        +-- Device
             |
             +-- CPU/GPU
*/
void printOpenCLInfo()
{
    cl_uint platformsCount;


    clGetPlatformIDs(
        0,
        NULL,
        &platformsCount);



    cl_platform_id* platforms =
        (cl_platform_id*)
        malloc(sizeof(cl_platform_id) *
               platformsCount);



    clGetPlatformIDs(
        platformsCount,
        platforms,
        NULL);



    printf("\nOpenCL Platforms: %u\n",
           platformsCount);



    for(cl_uint p=0; p<platformsCount; p++)
    {
        char name[256];


        clGetPlatformInfo(
            platforms[p],
            CL_PLATFORM_NAME,
            sizeof(name),
            name,
            NULL);


        printf("\nPlatform: %s\n", name);



        cl_uint devicesCount;


        clGetDeviceIDs(
            platforms[p],
            CL_DEVICE_TYPE_ALL,
            0,
            NULL,
            &devicesCount);



        cl_device_id* devices =
            (cl_device_id*)
            malloc(sizeof(cl_device_id) *
                   devicesCount);



        clGetDeviceIDs(
            platforms[p],
            CL_DEVICE_TYPE_ALL,
            devicesCount,
            devices,
            NULL);



        for(cl_uint d=0; d<devicesCount; d++)
        {
            char deviceName[256];


            clGetDeviceInfo(
                devices[d],
                CL_DEVICE_NAME,
                sizeof(deviceName),
                deviceName,
                NULL);


            printf("  Device: %s\n",
                   deviceName);
        }


        free(devices);
    }


    free(platforms);
}





void openclVectorAdd(
        float* A,
        float* B,
        float* C,
        int size)
{

    /*
        1. Select platform
    */

    cl_platform_id platform;


    clGetPlatformIDs(
        1,
        &platform,
        NULL);



    /*
        2. Select GPU device
    */

    cl_device_id device;


    clGetDeviceIDs(
        platform,
        CL_DEVICE_TYPE_GPU,
        1,
        &device,
        NULL);



    /*
        3. Create context

        Context = OpenCL environment
    */

    cl_context context =
        clCreateContext(
            NULL,
            1,
            &device,
            NULL,
            NULL,
            NULL);



    /*
        4. Command queue

        CPU sends commands here
    */

    cl_command_queue queue =
        clCreateCommandQueue(
            context,
            device,
            0,
            NULL);




    /*
        5. Allocate GPU memory

        CPU --> GPU
    */

    cl_mem bufA =
        clCreateBuffer(
            context,
            CL_MEM_READ_ONLY |
            CL_MEM_COPY_HOST_PTR,
            sizeof(float)*size,
            A,
            NULL);



    cl_mem bufB =
        clCreateBuffer(
            context,
            CL_MEM_READ_ONLY |
            CL_MEM_COPY_HOST_PTR,
            sizeof(float)*size,
            B,
            NULL);



    cl_mem bufC =
        clCreateBuffer(
            context,
            CL_MEM_WRITE_ONLY,
            sizeof(float)*size,
            NULL,
            NULL);




    /*
        6. Compile kernel
    */

    char* source =
        loadKernel(
            "vectorOp/src/vector_add.cl");



    cl_program program =
        clCreateProgramWithSource(
            context,
            1,
            (const char**)&source,
            NULL,
            NULL);



    clBuildProgram(
        program,
        1,
        &device,
        NULL,
        NULL,
        NULL);



    cl_kernel kernel =
        clCreateKernel(
            program,
            "vector_add",
            NULL);




    /*
        7. Kernel arguments
    */

    clSetKernelArg(kernel,0,sizeof(cl_mem),&bufA);

    clSetKernelArg(kernel,1,sizeof(cl_mem),&bufB);

    clSetKernelArg(kernel,2,sizeof(cl_mem),&bufC);




    /*
        8. Run GPU kernel
    */

    size_t globalSize = size;


    clEnqueueNDRangeKernel(
        queue,
        kernel,
        1,
        NULL,
        &globalSize,
        NULL,
        0,
        NULL,
        NULL);



    clFinish(queue);




    /*
        9. GPU --> CPU
    */

    clEnqueueReadBuffer(
        queue,
        bufC,
        CL_TRUE,
        0,
        sizeof(float)*size,
        C,
        0,
        NULL,
        NULL);




    /*
        10. Cleanup
    */

    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);

    clReleaseKernel(kernel);

    clReleaseProgram(program);

    clReleaseCommandQueue(queue);
    clReleaseContext(context);


    free(source);
}