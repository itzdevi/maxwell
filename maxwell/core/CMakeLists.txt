add_library(core STATIC
    src/mw_file.c
)
target_include_directories(core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)