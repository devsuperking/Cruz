cmake_minimum_required(VERSION 3.15)
project(CruzGui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE GUI_SOURCES src/*.cpp)

add_executable(CruzGui ${GUI_SOURCES})
target_link_libraries(CruzGui PRIVATE CruzCore)

if(EMSCRIPTEN)
    message(STATUS "CruzGui: Configuring for Emscripten/WebAssembly")
    set_target_properties(CruzGui PROPERTIES
        SUFFIX ".html"
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        LINK_FLAGS "-O2 -s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1"
    )
else()
    message(STATUS "CruzGui: Configuring for Desktop OpenGL")
    set_target_properties(CruzGui PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()
