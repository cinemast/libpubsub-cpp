cmake_minimum_required(VERSION 2.8)

file(GLOB pubsub_sources *.cpp)
file(GLOB pubsub_header *.h)

file(GLOB connector_sources connector/*.cpp)
file(GLOB connector_header connector/*.h)

add_library(pubsubcpp SHARED ${pubsub_sources} ${pubsub_header} ${connector_sources} ${connector_header})
target_link_libraries(pubsubcpp "-luuid -lpthread -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-server -ljsonrpccpp-client")
