cmake_minimum_required(VERSION 3.0.0)
project(open-mp VERSION 0.1.0)

include(CTest)
enable_testing()

set(SOURCE_FILES    open-mp/pch.h open-mp/BasicMatrix.h open-mp/ArrayMatrix.h open-mp/Array2DMatrix.h open-mp/VectorMatrix.h open-mp/Vector2DMatrix.h 
                    open-mp/MatrixUtils.h open-mp/Stopwatch.h open-mp/MatrixMultiplicator.h
                    ) 

add_executable(open-mp open-mp/main.cpp)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
