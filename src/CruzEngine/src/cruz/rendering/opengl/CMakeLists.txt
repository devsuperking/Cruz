file(GLOB_RECURSE OPENGL_SOURCES *.cpp *.h)
add_library(BackendOpenGL STATIC ${OPENGL_SOURCES})
target_include_directories(BackendOpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BackendOpenGL PUBLIC CruzCore)