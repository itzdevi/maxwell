find_package(Vulkan REQUIRED)

add_library(application STATIC
    src/mw_application.c
)
target_link_libraries(application PRIVATE
    Vulkan::Vulkan
    glfw
    cglm
    core
    renderer
)
target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)