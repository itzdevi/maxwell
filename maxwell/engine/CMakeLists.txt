add_library(engine STATIC
    src/mw_context.c
    src/core/mw_asset.c
    src/core/mw_ecs.c
    src/core/mw_window.c
    src/core/mw_engine_renderer.c
    src/ecs/mw_mesh_renderer.c
    src/ecs/mw_velocity_solver.c
    src/api/mw_time.c
    src/api/mw_component_builder.c
    src/api/mw_input.c
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(engine PRIVATE
    core
    renderer
    cglm
    flecs_static
    glfw
)