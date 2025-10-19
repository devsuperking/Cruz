cmake_minimum_required(VERSION 3.15)
project(CruzCore)

file(GLOB_RECURSE CORE_SOURCES src/*.cpp)
add_library(CruzCore STATIC ${CORE_SOURCES})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(CruzCore PUBLIC src)
target_compile_definitions(CruzCore PUBLIC GLFW_INCLUDE_NONE)

if(EMSCRIPTEN)
    message(STATUS "CruzCore: Emscripten build - not linking GLFW/GLAD")
else()
    message(STATUS "CruzCore: Desktop build - linking GLFW and GLAD")
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    add_subdirectory(thirdparty/glfw)
    add_subdirectory(thirdparty/glad)
    target_link_libraries(CruzCore PUBLIC glfw glad)
endif()
