# Find libjson-rpc-cpp
#
# Find the libjson-rpc-cpp includes and library
# 
# if you nee to add a custom library search path, do it via via CMAKE_PREFIX_PATH 
# 
# This module defines
#  JSONRPCCPP_INCLUDE_DIRS, where to find header, etc.
#  JSONRPCCPP_LIBRARIES, the libraries needed to use jsoncpp.
#  JSONRPCCPP_FOUND, If false, do not try to use jsoncpp.

# only look in default directories

#common
find_path(
    common_INCLUDE_DIR
    NAMES jsonrpccpp/common/errors.h
    DOC "libjson-rpc-cpp common include dir"
)

find_library(
    common_LIBRARY
    NAMES jsonrpccpp-common
    DOC "libjson-rpc-cpp common library"
)

find_path(
    client_INCLUDE_DIR
    NAMES jsonrpccpp/client.h
    DOC "libjson-rpc-cpp client include dir"
)

find_library(
    client_LIBRARY
    NAMES jsonrpccpp-client
    DOC "libjson-rpc-cpp client library"
)

find_path(
    server_INCLUDE_DIR
    NAMES jsonrpccpp/server.h
    DOC "libjson-rpc-cpp server include dir"
)

find_library(
    server_LIBRARY
    NAMES jsonrpccpp-server
    DOC "libjson-rpc-cpp server library"
)


set(JSONRPCCPP_INCLUDE_DIRS ${JSONRPCCPP_INCLUDE_DIR})
set(JSONRPCCPP_LIBRARIES ${common_LIBRARY} ${server_LIBRARY} ${client_LIBRARY})



# handle the QUIETLY and REQUIRED arguments and set JSONRPCCPP_FOUND to TRUE
# if all listed variables are TRUE, hide their existence from configuration view
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(jsonrpccpp DEFAULT_MSG
	JSONRPCCPP_INCLUDE_DIR JSONRPCCPP_LIBRARY)
mark_as_advanced (JSONRPCCPP_INCLUDE_DIR JSONRPCCPP_LIBRARY)

