cmake_minimum_required(VERSION 3.15)
project(Maxwell VERSION 1.0 LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        # -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -g
    )
    add_link_options(
        # -fsanitize=address,undefined
    )
endif()

add_subdirectory(maxwell)
add_subdirectory(sandbox)