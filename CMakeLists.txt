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

if(NOT MSVC)
    # Find Eigen3
    find_package(Eigen3 3.3 CONFIG)
    # find_package(Eigen3 3.3 CONFIG REQUIRED)
    # Find fmt
    find_package(fmt 5.3 CONFIG)
    # Find Catch2
    find_package(Catch2 2.9.2 CONFIG)
endif()
# When some aformentioned libraries cannot found.
# Then download source from web. 
add_subdirectory(extern)

add_library(src OBJECT 
    src/base/load.cc
    src/base/material.cc
    src/base/section.cc
    src/base/dof_handler.cc
    src/base/node.cc
    src/core/coord_tran.cc
    src/element/element_attr.cc
    src/element/element.cc
    src/element/additional.cc
    src/element/pipe.cc)


if(MSVC)
    if(TARGET Eigen3::Eigen)
        message(STATUS "Eigen3 intsalled")
    else()
        message(STATUS "${Eigen3_SOURCE_DIR} ${Eigen3_BINARY_DIR}")
    endif()
    if(TARGET fmt::fmt)
        message(STATUS "fmt installed")
    else()
        message(STATUS "${fmt_SOURCE_DIR} ${fmt_BINARY_DIR}")
    endif()
    if(TARGET Catch2::Catch)
        message(STATUS "Catch2 installed")
    else()
        message(STATUS "${Catch2_SOURCE_DIR} ${Catch2_BINARY_DIR}")
    endif()
    target_include_directories(src PUBLIC src/include ${Eigen3_SOURCE_DIR} ${fmt_SOURCE_DIR}/include)
    target_link_libraries(src PUBLIC OpenMP::OpenMP_CXX fmt::fmt)
else()   
    target_include_directories(src PUBLIC src/include)
    target_link_libraries(src PUBLIC OpenMP::OpenMP_CXX Eigen3::Eigen fmt::fmt)
endif()

# add_executable(gauss test/test_utility_01.cc)
# target_include_directories(gauss PUBLIC src/include)
# target_link_libraries(gauss PUBLIC Eigen3::Eigen OpenMP::OpenMP_CXX fmt::fmt )

enable_testing()

foreach(loopVar RANGE 1 9)
    if(${loopVar} GREATER 9)
        set(tName a${loopVar})
    else()
        set(tName a0${loopVar})
    endif()
    message(STATUS "**** Add test: ${tName}")
    add_executable(${tName} test/basic/${tName}.cc)
    target_include_directories(${tName} PUBLIC src/include ${Catch2_SOURCE_DIR}/single_include/catch2)
    target_link_libraries(${tName} PRIVATE src)
    add_test(NAME ${tName} COMMAND ${tName} --success)    
endforeach()


