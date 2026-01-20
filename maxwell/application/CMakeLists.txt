find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_library(application STATIC
    src/mw_application.c
)
target_link_libraries(application PRIVATE
    Vulkan::Vulkan
    glfw
    core
    math
    renderer
)
target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)