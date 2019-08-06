cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
# Project information 
project(CAFEA 
        VERSION 2.1.0
        DESCRIPTION "In-house FEA program"
        LANGUAGES CXX Fortran)

# Set for CXX standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# if(MSYS OR MINGW OR UNIX)
#     find_package(fmt 5.3 CONFIG REQUIRED)
#     if(fmt_FOUND)
#         message(STATUS "Find fmt")
#     endif()
# else()
#     include(FetchContent)
#     FetchContent_Declare(
#         fmt
#         GIT_REPOSITORY https://github.com/fmtlib/fmt.git 
#         GIT_TAG 5.3.0
#     )
#     message(STATUS "Begin downloading from GitHub")
#     FetchContent_MakeAvailable(fmt)
# endif()

# if(WIN32)
#     message(STATUS "This is Win32")
# endif()

# if(MSYS)
#     message(STATUS "This is MSYS")
# endif()

# if(MSVC)
#     message(STATUS "This is MSVC")
# endif()

# if(MINGW)
#     message(STATUS "This is MINGW")
# endif()
