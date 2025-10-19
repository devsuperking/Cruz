cmake_minimum_required(VERSION 3.15)
project(CruzEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glad)

add_executable(CruzEngine src/main.cpp)
target_link_libraries(CruzEngine PRIVATE glfw glad)