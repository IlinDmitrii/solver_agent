cmake_minimum_required(VERSION 3.1)

project(solver_agent)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(SOLVTEST_SRC main.cpp)

find_package(MPILibraries REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

add_executable(${PROJECT_NAME}  ${SOLVTEST_SRC})
target_link_libraries(${PROJECT_NAME} ${MPI_C_LIBRARIES})
