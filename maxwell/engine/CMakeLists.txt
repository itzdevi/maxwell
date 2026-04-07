add_library(engine STATIC
    src/mw_context.c
    src/core/mw_asset.c
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(engine PRIVATE
    core
    application
    cglm
)