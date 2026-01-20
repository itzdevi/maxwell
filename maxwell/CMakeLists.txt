cmake_minimum_required(VERSION 3.15.0)
project(maxwell LANGUAGES C)

add_subdirectory(core)
add_subdirectory(math)
add_subdirectory(application)
add_subdirectory(renderer)

add_library(maxwell INTERFACE)
target_link_libraries(maxwell INTERFACE
    core
    math
    application
    renderer
)
target_include_directories(maxwell INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)