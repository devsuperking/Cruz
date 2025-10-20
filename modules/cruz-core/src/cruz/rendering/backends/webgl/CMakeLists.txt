file(GLOB_RECURSE WEBGL_SOURCES *.cpp *.h)
add_library(BackendWebGL STATIC ${WEBGL_SOURCES})
target_include_directories(BackendWebGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BackendWebGL PUBLIC CruzCore)