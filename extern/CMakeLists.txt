include(FetchContent)

set(EXT_HEAD "Extern library: ")

if(TARGET Eigen3::Eigen)
    message(STATUS "Find Eigen3")
else()
    message(STATUS "${EXT_HEAD}Cannot find Eigen3")
    message(STATUS "${EXT_HEAD}Try to download Eigen3 from github")
    FetchContent_Declare(
        Eigen3
        URL      https://bitbucket.org/eigen/eigen/get/3.3.7.tar.gz
        URL_HASH MD5=f2a417d083fe8ca4b8ed2bc613d20f07
    )
    # Clone git repository is a time consuming task!!!
    # FetchContent_Declare(
    #     Eigen3
    #     GIT_REPOSITORY https://github.com/eigenteam/eigen-git-mirror.git
    #     GIT_TAG 3.3.7
    # )
    FetchContent_MakeAvailable(Eigen3)
endif()

if(TARGET fmt::fmt)#(fmt_FOUND)
    message(STATUS "Find fmt")
else()
    message(STATUS "${EXT_HEAD}Try to download fmt from github")
    FetchContent_Declare(
        fmt
        URL      https://github.com/fmtlib/fmt/releases/download/6.0.0/fmt-6.0.0.zip
        URL_HASH MD5=6b8cac665e3c48b6c7b0ca68d0d023fd # Hash of file not string of URL!
    )
    # FetchContent_Declare(
    #     fmt
    #     GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    #     GIT_TAG 5.3.0
    # )
    message(STATUS "${EXT_HEAD}Begin downloading...")
    FetchContent_MakeAvailable(fmt)
endif()

if(TARGET Catch2::Catch2)
    message(STATUS "Find Catch2")
else()
    message(STATUS "${EXT_HEAD}Try to download Catch2 from github")
    FetchContent_Declare(
        Catch2
        URL https://github.com/catchorg/Catch2/archive/v2.10.0.tar.gz
        URL_HASH MD5=c639cae2f052cdaaaafc12b1a706bd99 # Hash of file not string of URL!
        # GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        # GIT_TAG v2.9.2
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
