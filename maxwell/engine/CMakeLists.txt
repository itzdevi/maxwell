add_library(engine STATIC
    src/mw_context.c
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(engine PRIVATE
    application
    cglm
)