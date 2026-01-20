cmake_minimum_required(VERSION 3.15.0)
project(sandbox LANGUAGES C)

add_executable(sandbox
    main.c
)

target_link_libraries(sandbox PRIVATE
    maxwell
)