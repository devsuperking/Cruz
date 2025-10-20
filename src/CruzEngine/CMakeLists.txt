cmake_minimum_required(VERSION 3.15)
project(CruzCore)

file(GLOB_RECURSE CORE_SOURCES src/cruz/common/*.cpp src/cruz/common/*.h)

if(NOT CORE_SOURCES)
    message(FATAL_ERROR "No source files found in cruz-core/src")
endif()

add_library(CruzCore STATIC ${CORE_SOURCES})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(CruzCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

get_filename_component(EMSCRIPTEN_INCLUDE "${CMAKE_SOURCE_DIR}/../thirdparty/upstream/emscripten/cache/sysroot/include" ABSOLUTE)
target_include_directories(CruzCore PUBLIC "${EMSCRIPTEN_INCLUDE}")

target_compile_definitions(CruzCore PUBLIC GLFW_INCLUDE_NONE)

if(EMSCRIPTEN)
    message("Detected backend: EMSCRIPTEN")
    target_compile_definitions(CruzCore PUBLIC EMSCRIPTEN)
else()
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    add_subdirectory(thirdparty/glfw)
    add_subdirectory(thirdparty/glad)
    target_link_libraries(CruzCore PUBLIC glfw glad)
endif()

if(EMSCRIPTEN)
    add_subdirectory(src/cruz/rendering/backends/webgl)
    target_link_libraries(CruzCore PUBLIC BackendWebGL)
else()
    add_subdirectory(src/cruz/rendering/backends/opengl)
    target_link_libraries(CruzCore PUBLIC BackendOpenGL)
endif()
