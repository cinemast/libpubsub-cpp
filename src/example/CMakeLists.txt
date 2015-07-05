cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/src)

set(link_libraries "-luuid -lpthread -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-server -ljsonrpccpp-client")


INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/AbstractMonitor.h ${CMAKE_BINARY_DIR}/AirspeedSensor.h ${CMAKE_BINARY_DIR}/HumiditySensor.h
        COMMAND pubsubstub ARGS ${CMAKE_CURRENT_SOURCE_DIR}/peers.json
        MAIN_DEPENDENCY peers.json
        DEPENDS pubsubstub
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating Peer Stubs"
        VERBATIM
)

add_custom_target(common_stubs
        DEPENDS ${CMAKE_BINARY_DIR}/AbstractMonitor.h ${CMAKE_BINARY_DIR}/AirSpeedSensor.h ${CMAKE_BINARY_DIR}/HumiditySensor.h
)

add_executable(monitor monitor.cpp ${CMAKE_BINARY_DIR}/AbstractMonitor.h)
target_link_libraries(monitor ${link_libraries} pubsubcpp)

add_executable(airsensor airsensor.cpp ${CMAKE_BINARY_DIR}/AirspeedSensor.h)
target_link_libraries(airsensor ${link_libraries} pubsubcpp)

add_executable(humiditysensor humiditysensor.cpp ${CMAKE_BINARY_DIR}/HumiditySensor.h)
target_link_libraries(humiditysensor ${link_libraries} pubsubcpp)

add_executable(tempsensor tempsensor.cpp ${CMAKE_BINARY_DIR}/TempSensor.h)
target_link_libraries(tempsensor ${link_libraries} pubsubcpp)

