cmake_minimum_required(VERSION 3.10)
project ( 
    cmake-generator 
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
include_directories(include)
add_executable(cmake-generator ${all_SRCS})
set(SOURCE_DIR src)
set(BUILD_DIR build)
set_target_properties(cmake-generator PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
