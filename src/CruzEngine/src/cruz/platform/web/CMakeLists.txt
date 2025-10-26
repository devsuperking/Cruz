file(GLOB_RECURSE WEBPLATFORM_SRC *.cpp *.h)
add_library(WebPlatform STATIC ${WEBPLATFORM_SRC})
target_include_directories(WebPlatform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(WebPlatform PUBLIC CruzCore)