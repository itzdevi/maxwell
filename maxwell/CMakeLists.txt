cmake_minimum_required(VERSION 3.15.0)
project(maxwell LANGUAGES C)

find_package(Vulkan REQUIRED)

add_subdirectory(core)
add_subdirectory(gpu)
add_subdirectory(renderer)
add_subdirectory(engine)
add_subdirectory(cglm)
add_subdirectory(glfw)
add_subdirectory(flecs)

add_library(maxwell INTERFACE)
target_link_libraries(maxwell INTERFACE
    engine
    cglm
)
target_include_directories(maxwell INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)