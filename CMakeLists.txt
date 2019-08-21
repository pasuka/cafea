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

# Find OpenMP for Eigen3
find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
    message(STATUS "Find OpenMP")
else()
    message(STATUS "Cannot find OpenMP")
endif()

# Find Eigen3
find_package(Eigen3 3.3 CONFIG)
# find_package(Eigen3 3.3 CONFIG REQUIRED)

# Find fmt
find_package(fmt 5.3 CONFIG)

# Find Catch2
find_package(Catch2 2.9.2 CONFIG)
# When some aformentioned libraries cannot found.
# Then download source from web. 
add_subdirectory(extern)

message(STATUS "${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR}")

add_executable(a01 test/test_utility_01.cc)
target_include_directories(a01 PUBLIC src/include ${Catch2_SOURCE_DIR}/include)
target_link_libraries(a01 PRIVATE Eigen3::Eigen fmt::fmt )


