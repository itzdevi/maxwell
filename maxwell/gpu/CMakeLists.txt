add_library(gpu STATIC
    src/mw_gpu.c
    src/mw_command.c
    src/mw_device.c
    src/mw_instance.c
    src/mw_physical_device.c
    src/mw_queue.c
)

target_link_libraries(gpu PUBLIC
    Vulkan::Vulkan
    core
)

target_include_directories(gpu
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)