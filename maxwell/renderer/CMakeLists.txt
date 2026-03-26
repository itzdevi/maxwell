add_library(renderer STATIC
    src/mw_renderer.c
    src/core/mw_instance.c
    src/core/mw_surface.c
    src/core/mw_physical_device.c
    src/core/mw_device.c
    src/core/mw_queue.c
    src/renderer/mw_swapchain.c
    src/renderer/mw_pipeline.c
    src/renderer/mw_framebuffer.c
    src/renderer/mw_sync_objects.c
    src/renderer/mw_vertex.c
    src/commands/mw_command.c
    src/commands/mw_draw_command.c
    src/commands/mw_transfer_command.c
    src/resources/mw_buffer.c
)

find_package(Vulkan REQUIRED)

target_link_libraries(renderer PUBLIC
    Vulkan::Vulkan
    core
    math
    application
)

target_include_directories(renderer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)