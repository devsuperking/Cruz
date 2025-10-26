file(GLOB_RECURSE WINDOWSPLATFORM_SRC *.cpp *.h)
add_library(WindowsPlatform STATIC ${WINDOWSPLATFORM_SRC})
target_include_directories(WindowsPlatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(WindowsPlatform PUBLIC CruzCore)