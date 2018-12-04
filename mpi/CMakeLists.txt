cmake_minimum_required(VERSION 2.8)

project(mpi-matrix)

add_executable(mpi-matrix main.cpp)

# Require MPI for this project:
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(MPI_INCLUDE_PATH)
SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)
target_link_libraries(mpi-matrix ${MPI_LIBRARIES})
