add_library(renderer STATIC
    src/mw_renderer.c
    src/mw_instance.c
    src/mw_surface.c
    src/mw_physical_device.c
    src/mw_swapchain.c
    src/mw_device.c
    src/mw_render_queue.c
    src/mw_pipeline.c
    src/mw_framebuffer.c
    src/mw_command_buffer.c
    src/mw_sync_objects.c
)

find_package(Vulkan REQUIRED)

target_link_libraries(renderer PUBLIC
    Vulkan::Vulkan
    core
    math
    application
)

target_include_directories(renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)