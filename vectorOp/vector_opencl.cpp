#include "vectorOp.h"

#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>



static cl_context context = NULL;

static cl_command_queue queue = NULL;

static cl_program program = NULL;

static cl_device_id device = NULL;



/*
==============================================================================
Timer
==============================================================================
*/

static double time_ms()
{
    static auto start =
        std::chrono::high_resolution_clock::now();


    auto now =
        std::chrono::high_resolution_clock::now();


    return std::chrono::duration<double,std::milli>(
            now-start).count();
}



/*
==============================================================================
Error helper
==============================================================================
*/

static void check_error(
        const char* text,
        cl_int error)
{

    if(error == CL_SUCCESS)

        printf("[SUCCESS] %s\n",text);

    else

        printf("[FAILED] %s (%d)\n",
                text,
                error);

}



/*
==============================================================================
Load OpenCL kernel file
==============================================================================
*/

static char* load_kernel(
        const char* filename)
{

    FILE* file =
        fopen(filename,"rb");


    if(!file)
    {
        printf("Cannot open kernel file\n");
        return NULL;
    }



    fseek(file,0,SEEK_END);


    long size =
        ftell(file);


    rewind(file);



    char* source =
        (char*)malloc(size+1);



    fread(
        source,
        1,
        size,
        file);



    source[size]=0;


    fclose(file);



    return source;

}





/*
==============================================================================
Initialize OpenCL
==============================================================================
*/

static int initialize_opencl()
{

    cl_int error;



    printf("\n");
    printf("====================================\n");
    printf(" OpenCL Initialization\n");
    printf("====================================\n");



    cl_platform_id platform;



    error =
    clGetPlatformIDs(
        1,
        &platform,
        NULL);



    check_error(
        "Platform",
        error);



    error =
    clGetDeviceIDs(
        platform,
        CL_DEVICE_TYPE_GPU,
        1,
        &device,
        NULL);



    check_error(
        "Device",
        error);



    char name[256];


    clGetDeviceInfo(
        device,
        CL_DEVICE_NAME,
        sizeof(name),
        name,
        NULL);



    printf("[INFO] Device : %s\n",name);



    context =
    clCreateContext(
        NULL,
        1,
        &device,
        NULL,
        NULL,
        &error);



    check_error(
        "Context",
        error);




    queue =
    clCreateCommandQueue(
        context,
        device,
        0,
        &error);



    check_error(
        "Queue",
        error);




    char* source =
        load_kernel(
            "VectorOp/vector_operations.cl");



    if(!source)
        return -1;



    size_t length =
        strlen(source);



    program =
    clCreateProgramWithSource(
        context,
        1,
        (const char**)&source,
        &length,
        &error);



    free(source);



    check_error(
        "Program",
        error);




    error =
    clBuildProgram(
        program,
        1,
        &device,
        NULL,
        NULL,
        NULL);



    check_error(
        "Build",
        error);



    printf("====================================\n");
    printf(" OpenCL Ready\n");
    printf("====================================\n");



    return 0;

}







/*
==============================================================================
Generic Kernel Execution
==============================================================================
*/

static void execute_kernel(

        VectorOperation op,

        const char* kernel_name,

        float* inputA,

        float* inputB,

        float* output,

        int size,

        OpenCLTiming* timing)

{

    printf("\nKernel : %s\n",
            kernel_name);



    double total_start =
        time_ms();



    cl_int error;



    cl_kernel kernel =
    clCreateKernel(
        program,
        kernel_name,
        &error);




    cl_mem buffer_A =
    clCreateBuffer(
        context,
        CL_MEM_READ_ONLY |
        CL_MEM_COPY_HOST_PTR,
        sizeof(float)*size,
        inputA,
        &error);




    cl_mem buffer_C =
    clCreateBuffer(
        context,
        CL_MEM_WRITE_ONLY,
        sizeof(float)*size,
        NULL,
        &error);




    cl_mem buffer_B = NULL;




    if(op == ADD ||
       op == SUBTRACT ||
       op == MULTIPLY ||
       op == DIVIDE ||
       op == DOT_PRODUCT)
    {


        buffer_B =
        clCreateBuffer(
            context,
            CL_MEM_READ_ONLY |
            CL_MEM_COPY_HOST_PTR,
            sizeof(float)*size,
            inputB,
            &error);

    }




    int arg=0;



    clSetKernelArg(
        kernel,
        arg++,
        sizeof(cl_mem),
        &buffer_A);



    if(buffer_B)
    {

        clSetKernelArg(
            kernel,
            arg++,
            sizeof(cl_mem),
            &buffer_B);

    }




    clSetKernelArg(
        kernel,
        arg++,
        sizeof(cl_mem),
        &buffer_C);




    if(op == SUM_REDUCTION ||
       op == DOT_PRODUCT)
    {

        clSetKernelArg(
            kernel,
            arg++,
            sizeof(int),
            &size);

    }




    size_t global_size=size;



    if(op == SUM_REDUCTION ||
       op == DOT_PRODUCT)

        global_size=1;




    /*
    Kernel timing only
    */

    double kernel_start =
        time_ms();



    error =
    clEnqueueNDRangeKernel(
        queue,
        kernel,
        1,
        NULL,
        &global_size,
        NULL,
        0,
        NULL,
        NULL);



    clFinish(queue);



    timing->kernel_time_ms =
        time_ms() - kernel_start;






    clEnqueueReadBuffer(
        queue,
        buffer_C,
        CL_TRUE,
        0,
        sizeof(float)*size,
        output,
        0,
        NULL,
        NULL);





    timing->total_time_ms =
        time_ms() - total_start;



    clReleaseMemObject(buffer_A);

    clReleaseMemObject(buffer_C);


    if(buffer_B)
        clReleaseMemObject(buffer_B);


    clReleaseKernel(kernel);

}






/*
==============================================================================
Public API
==============================================================================
*/


void vector_opencl_execute(

        VectorOperation op,

        float* inputA,

        float* inputB,

        float* output,

        int size,

        OpenCLTiming* timing)

{


    if(context == NULL)
    {

        if(initialize_opencl()!=0)
            return;

    }



    const char* kernel=NULL;



    switch(op)
    {


    case ADD:
        kernel="vector_add_kernel";
        break;


    case SUBTRACT:
        kernel="vector_subtract_kernel";
        break;


    case MULTIPLY:
        kernel="vector_multiply_kernel";
        break;


    case DIVIDE:
        kernel="vector_divide_kernel";
        break;


    case SQUARE:
        kernel="vector_square_kernel";
        break;


    case SQRT:
        kernel="vector_sqrt_kernel";
        break;


    case EXP:
        kernel="vector_exp_kernel";
        break;


    case LOG:
        kernel="vector_log_kernel";
        break;


    case RELU:
        kernel="vector_relu_kernel";
        break;


    case SIGMOID:
        kernel="vector_sigmoid_kernel";
        break;


    case TANH:
        kernel="vector_tanh_kernel";
        break;


    case SUM_REDUCTION:
        kernel="vector_sum_reduction_kernel";
        break;


    case DOT_PRODUCT:
        kernel="vector_dot_product_kernel";
        break;


    case COPY:
        kernel="vector_copy_kernel";
        break;


    case FILL:
        kernel="vector_fill_kernel";
        break;


    }



    if(kernel)
    {

        execute_kernel(
            op,
            kernel,
            inputA,
            inputB,
            output,
            size,
            timing);

    }

}
