file(GLOB_RECURSE OPENGL_SOURCES *.cpp *.h)
add_library(PrimitiveRenderer STATIC ${OPENGL_SOURCES})
target_include_directories(PrimitiveRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(PrimitiveRenderer PUBLIC CruzCore)