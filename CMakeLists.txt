cmake_minimum_required(VERSION 3.20)

project(OpenCLTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCL REQUIRED)

add_executable(OpenCLTest

    main.cpp

    vectorOp/src/vectorOp.cpp
    vectorOp/src/cpu_vector.cpp
    vectorOp/src/opencl_vector.cpp
)

target_include_directories(OpenCLTest PRIVATE

    ${OpenCL_INCLUDE_DIRS}

    vectorOp/include
)

target_link_libraries(OpenCLTest PRIVATE

    OpenCL::OpenCL

)