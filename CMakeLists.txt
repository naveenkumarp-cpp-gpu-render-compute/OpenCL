cmake_minimum_required(VERSION 3.20)

project(OpenCLTest LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)



# OpenCL locations

set(OPENCL_INCLUDE_DIR
"C:/Users/NaveenKumar/OneDrive - MULTICOREWARE/Desktop/study/OpenCL/code/OpenCL-Headers")


set(OPENCL_LIBRARY
"C:/Users/NaveenKumar/OneDrive - MULTICOREWARE/Desktop/study/OpenCL/code/OpenCL-SDK/build/external/OpenCL-ICD-Loader/Release/OpenCL.lib")



add_executable(OpenCLTest

    main.cpp

    vectorOp/src/vectorOp.cpp
    vectorOp/src/cpu_vector.cpp
    vectorOp/src/opencl_vector.cpp
)


target_include_directories(OpenCLTest PRIVATE

    ${OPENCL_INCLUDE_DIR}

    vectorOp/include
)


target_link_libraries(OpenCLTest PRIVATE

    ${OPENCL_LIBRARY}

)
