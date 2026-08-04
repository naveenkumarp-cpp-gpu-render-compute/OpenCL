cmake_minimum_required(VERSION 3.20)

project(OpenCLTest LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# ============================================================
# OpenCL Detection
#
# Priority:
#
# 1. System OpenCL
#    NVIDIA / Intel / AMD drivers
#
# 2. Local Khronos SDK
#
# ============================================================


find_package(OpenCL QUIET)



if(OpenCL_FOUND)

    message("================================")
    message("Using system OpenCL")
    message("OpenCL include:")
    message(${OpenCL_INCLUDE_DIRS})
    message("OpenCL library:")
    message(${OpenCL_LIBRARIES})
    message("================================")


else()


    message("System OpenCL not found")
    message("Trying local OpenCL SDK")



    set(LOCAL_OPENCL_HEADERS

        "${CMAKE_CURRENT_SOURCE_DIR}/../OpenCL-Headers"

    )


    set(LOCAL_OPENCL_LIBRARY

        "${CMAKE_CURRENT_SOURCE_DIR}/../OpenCL-SDK/build/external/OpenCL-ICD-Loader/Release/OpenCL.lib"

    )



    if(NOT EXISTS ${LOCAL_OPENCL_HEADERS}/CL/cl.h)

        message(FATAL_ERROR

        "OpenCL headers not found"

        )

    endif()



    if(NOT EXISTS ${LOCAL_OPENCL_LIBRARY})

        message(FATAL_ERROR

        "OpenCL library not found"

        )

    endif()



    set(OpenCL_INCLUDE_DIRS

        ${LOCAL_OPENCL_HEADERS}

    )


    set(OpenCL_LIBRARIES

        ${LOCAL_OPENCL_LIBRARY}

    )


    message("================================")
    message("Using local OpenCL SDK")
    message("Headers:")
    message(${OpenCL_INCLUDE_DIRS})
    message("Library:")
    message(${OpenCL_LIBRARIES})
    message("================================")


endif()





# ============================================================
# Sources
# ============================================================


add_executable(OpenCLTest


    main.cpp


    vectorOp/vectorOp.cpp

  


    vectorOp/vector_cpu.cpp


    vectorOp/vector_opencl.cpp



)





# ============================================================
# Include directories
# ============================================================


target_include_directories(OpenCLTest PRIVATE


    ${OpenCL_INCLUDE_DIRS}


    vectorOp/include


)





# ============================================================
# Link OpenCL
# ============================================================


target_link_libraries(OpenCLTest PRIVATE


    ${OpenCL_LIBRARIES}


)
