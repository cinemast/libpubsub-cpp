cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(pubsubstub main.cpp peerstubgenerator.h peerstubgenerator.cpp)
target_link_libraries(pubsubstub "-ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-stub")
