cmake_minimum_required(VERSION 3.10)
project(bsort)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bsort main.cpp)

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RAYLIB REQUIRED raylib)
    target_include_directories(bsort PRIVATE ${RAYLIB_INCLUDE_DIRS})
    target_link_directories(bsort PRIVATE ${RAYLIB_LIBRARY_DIRS})
endif()

target_link_libraries(bsort
    raylib
    m
    dl
    pthread
    X11
)
