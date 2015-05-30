cmake_minimum_required(VERSION 2.8)
project(libpubsub-cpp)

file(GLOB sources *.cpp)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

add_executable(test ${sources})
target_link_libraries(test "-lpthread")
