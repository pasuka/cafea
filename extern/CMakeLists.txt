include(FetchContent)

# Find OpenMP for Eigen3
find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
    message(STATUS "Find OpenMP")
else()
    message(STATUS "Cannot find OpenMP")
endif()

# Find Boost
find_package(Boost 1.70 COMPONENTS filesystem)
if(Boost_FOUND)
    message(STATUS "Find Boost")
else()
    message(STATUS "Cannot find Boost")
endif()

# Find Eigen3
find_package(Eigen3 3.3 CONFIG)
if(TARGET Eigen3::Eigen)
    message(STATUS "Find Eigen3")
else()
    message(STATUS "Cannot find Eigen3")
    message(STATUS "Try to download from github")
    FetchContent_Declare(
        Eigen3
        GIT_REPOSITORY https:github.com/eigenteam/eigen-git-mirror.git
        GIT_TAG 3.3.7
    )
    FetchContent_MakeAvailable(Eigen3)
endif()

# Find fmt
find_package(fmt 5.3 CONFIG)
if(fmt_FOUND)
    message(STATUS "Find fmt")
else()
    message(STATUS "Cannot find fmt")
    message(STATUS "Try to download from github")
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 5.3.0
    )
    message(STATUS "Begin downloading...")
    FetchContent_MakeAvailable(fmt)
endif()

# Find Catch2
find_package(Catch2 2.9.1 CONFIG)
if(Catch2_FOUND)
    message(STATUS "Find Catch2")
else()
    message(STATUS "Try to download from github")
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.9.1
    )
    message(STATUS "Begin downloading...")
    FetchContent_MakeAvailable(Catch2)
endif()
# if(MSYS OR MINGW OR UNIX)
#     find_package(fmt 5.3 CONFIG REQUIRED)
#     if(fmt_FOUND)
#         message(STATUS "Find fmt")
#     endif()
# else()
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
