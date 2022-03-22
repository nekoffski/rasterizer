cmake_minimum_required(VERSION 3.10)
project(rasterizer)

add_definitions(-DSPDLOG_COMPILED_LIB=1)
add_subdirectory(3rdparty)
add_subdirectory(src)