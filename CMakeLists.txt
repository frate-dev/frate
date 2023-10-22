cmake_minimum_required(VERSION 3.10)
project ( 
    cmake-generator 
    VERSION 0.0.1
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)
function(CloneRepository repositoryURL branchName projectName sourceDir)
  #Commands are left empty so that we only checkout the source and no not perform any kind of build
  message("Starting to clone ${projectName} into ${sourceDir}")
  FetchContent_Declare(
    "${projectName}"
    GIT_REPOSITORY "${repositoryURL}"
    SOURCE_DIR "${sourceDir}"
    GIT_TAG "origin/${branchName}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  FetchContent_MakeAvailable(${projectName})
endfunction(CloneRepository)


#file(GLOB LIB_SRCS
#  "./src/Command/*.cpp"
#  "./src/Command/*.hpp"
#  "./src/Command/*.h"
#)
#add_library(Command 
#  ${LIB_SRCS}
#)


file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR}
  "include/**.h"
  "include/**.hpp"
  "src/**.cpp"
  "src/**.c"
  #"${PROJECT_SOURCE_DIR}/include/*.h"
  #"${PROJECT_SOURCE_DIR}/include/*.hpp"
  #"${PROJECT_SOURCE_DIR}/src/*.cpp"
  #"${PROJECT_SOURCE_DIR}/src/*.c"
)

message("Sources: ${SOURCES}")



include_directories(include)
add_executable(cmake-generator ${SOURCES})
set(SOURCE_DIR src)
set(BUILD_DIR build)


set_target_properties(cmake-generator PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
