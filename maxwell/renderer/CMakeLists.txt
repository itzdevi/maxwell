add_library(renderer STATIC
    src/mw_renderer.c
    src/core/mw_surface.c
    src/core/mw_descriptor.c
    src/renderer/mw_swapchain.c
    src/renderer/mw_pipeline.c
    src/renderer/mw_framebuffer.c
    src/renderer/mw_sync_objects.c
    src/renderer/mw_vertex.c
    src/renderer/mw_render_queue.c
    src/resources/mw_uniform_buffer.c
    src/commands/mw_draw_command.c
    src/resources/mw_buffer.c
    src/resources/mw_vertex_buffer.c
    src/resources/mw_index_buffer.c
    src/resources/mw_mesh.c
)


target_link_libraries(renderer PUBLIC
    Vulkan::Vulkan
    core
    cglm
    gpu
)

target_include_directories(renderer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)