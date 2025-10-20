cmake_minimum_required(VERSION 3.15)

add_library(glad STATIC
    src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(glad PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)