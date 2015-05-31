cmake_minimum_required(VERSION 2.8)
project(libpubsub-cpp)

file(GLOB sources *.cpp)
file(GLOB header *.h)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

add_subdirectory(src/pubsubcpp)
add_subdirectory(src/example)


