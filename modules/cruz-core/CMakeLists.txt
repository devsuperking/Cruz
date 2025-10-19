cmake_minimum_required(VERSION 3.15)
file(GLOB_RECURSE CORE_SOURCES src/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glad)

add_library(CruzCore STATIC ${CORE_SOURCES})
target_include_directories(CruzCore PUBLIC src)
target_link_libraries(CruzCore PUBLIC glfw glad)