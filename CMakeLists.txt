cmake_minimum_required(VERSION 2.6)
project(Minimize)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin/lib
        CACHE PATH
        "Libraries")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin/run
        CACHE PATH
        "For tests & executables")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/arch
        CACHE PATH
        "Others"
    )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

enable_testing()

find_package(Boost REQUIRED)

include_directories(headers)

add_subdirectory(test build/test )