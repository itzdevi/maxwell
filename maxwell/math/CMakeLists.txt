add_library(math STATIC
    src/mw_vec2.c
    src/mw_math_func.c
)
target_link_libraries(math PRIVATE
    core
)
target_include_directories(math
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)