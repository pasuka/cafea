include(FetchContent)

set(EXT_HEAD "Extern library: ")

if(TARGET Eigen3::Eigen)
    message(STATUS "Find Eigen3")
else()
    message(STATUS "${EXT_HEAD}Cannot find Eigen3")
    message(STATUS "${EXT_HEAD}Try to download Eigen3 from github")
    FetchContent_Declare(
        Eigen3
        GIT_REPOSITORY https://github.com/eigenteam/eigen-git-mirror.git
        GIT_TAG 3.3.7
    )
    FetchContent_MakeAvailable(Eigen3)
endif()

if(TARGET fmt::fmt)#(fmt_FOUND)
    message(STATUS "Find fmt")
else()
    message(STATUS "${EXT_HEAD}Try to download fmt from github")
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 5.3.0
    )
    message(STATUS "${EXT_HEAD}Begin downloading...")
    FetchContent_MakeAvailable(fmt)
endif()

if(TARGET Catch2::Catch2)
    message(STATUS "Find Catch2")
else()
    message(STATUS "${EXT_HEAD}Try to download Catch2 from github")
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.9.2
    )
    message(STATUS "${EXT_HEAD}Begin downloading...")
    FetchContent_MakeAvailable(Catch2)
endif()
# # Find Boost
# find_package(Boost 1.70 COMPONENTS filesystem)
# if(Boost_FOUND)
#     message(STATUS "${EXT_HEAD}Find Boost")
# else()
#     message(STATUS "${EXT_HEAD}Cannot find Boost")
# endif()
