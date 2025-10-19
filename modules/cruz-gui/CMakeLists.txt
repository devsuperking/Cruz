cmake_minimum_required(VERSION 3.15)
project(CruzGui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CruzGui src/main.cpp)
set_target_properties(CruzGui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
target_link_libraries(CruzGui CruzCore)