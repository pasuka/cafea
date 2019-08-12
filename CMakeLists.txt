cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# # Set MSYS generator
# set(CMAKE_GENERATOR "MSYS Makefiles")
# message(STATUS "generator is set to ${CMAKE_GENERATOR}")

# Project information 
project(CAFEA 
        VERSION 2.1.0
        DESCRIPTION "In-house FEA program"
        LANGUAGES CXX)

# Set for CXX standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set Fortran language
if(MSYS OR MINGW OR UNIX)
    enable_language(Fortran)
    message(STATUS "Add Fortran language")
endif()
# Set for Eigen OpenMP fmt Boost library
add_subdirectory(extern)

# find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
    message(STATUS "Find OpenMP")
else()
    message(STATUS "Cannot find OpenMP")
endif()

# find_package(Eigen3 3.3 CONFIG REQUIRED)
if(TARGET Eigen3::Eigen)
    message(STATUS "Find Eigen3")
else()
    message(STATUS "Cannot find Eigen3")
endif()
message("${Eigen3_SOURCE_DIR}, $ENV{Eigen3_BINARY_DIR}")
message(STATUS "${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR}")
