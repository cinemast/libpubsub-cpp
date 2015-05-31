cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(client client.cpp)
target_link_libraries(client "-luuid -lpthread -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-server -ljsonrpccpp-client")
target_link_libraries(client pubsubcpp)
