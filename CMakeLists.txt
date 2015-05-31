cmake_minimum_required(VERSION 2.8)
project(libpubsub-cpp)

file(GLOB sources *.cpp)
file(GLOB header *.h)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

add_executable(test ${sources} ${header})
target_link_libraries(test "-luuid -lpthread -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-server -ljsonrpccpp-client")


add_executable(client client.cpp)
target_link_libraries(client "-luuid -lpthread -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-server -ljsonrpccpp-client")
