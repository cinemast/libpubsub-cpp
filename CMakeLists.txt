cmake_minimum_required(VERSION 2.8)
project(libpubsub-cpp)

file(GLOB sources *.cpp)
file(GLOB header *.h)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Jsoncpp REQUIRED)
find_package(Jsonrpccpp COMPONENTS common server client REQUIRED)

add_subdirectory(src/pubsubcpp)
add_subdirectory(src/pubsubstub)
add_subdirectory(src/example)


