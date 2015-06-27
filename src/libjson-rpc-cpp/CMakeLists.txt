cmake_minimum_required(VERSION 2.8)
project(libjsonrpccpp_builder CXX)
include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    libjsonrpccpp
    PREFIX ${CMAKE_BINARY_DIR}/libjson-rpc-cpp
    GIT_REPOSITORY https://github.com/cinemast/libjson-rpc-cpp.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    #CONFIGURE_COMMAND "mkdir build && cd build && cmake .."
    #BUILD_COMMAND "make"
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

ExternalProject_Get_Property(libjsonrpccpp source_dir)
set(LIBJSONRPCCPP_INCLUDE_DIR ${source_dir}/src CACHE INTERNAL "Path to include folder for libjson-rpc-cpp")
