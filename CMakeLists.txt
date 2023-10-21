cmake_minimum_required(VERSION 3.20.0)
project ( 
    test-project 
    VERSION 0.0.1
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)
file(GLOB all_SRCS
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/include/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.c"
)
include_directories(./include)
add_executable(test-project ${all_SRCS})
set(SOURCE_DIR ./src)
set(BUILD_DIR ./build)
