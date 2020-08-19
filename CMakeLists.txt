CMAKE_MINIMUM_REQUIRED (VERSION 3.10)
PROJECT (SimpleRPC)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
SET( CMAKE_CXX_STANDARD 11)
SET( CMAKE_CXX_STANDARD_REQUIRED ON )
ADD_COMPILE_OPTIONS ( -g -Wall )

#-------------------------------------------------------------------------------
# Set Third party Library
#-------------------------------------------------------------------------------
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

find_package(Boost 1.68.0 REQUIRED COMPONENTS system log)

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
# SET(SIMPLE_RPC_TARGET_NAME ${PROJECT_NAME})

#-------------------------------------------------------------------------------
# Set Build Variable
#-------------------------------------------------------------------------------
SET(PROJECT_DIR ${PROJECT_SOURCE_DIR})
SET(BUILD_DIR_EXAMPLES ${CMAKE_SOURCE_DIR}/build/bin)

SET(SRPC_INCLUDES
    ${PROJECT_DIR}/include
)

SET(SRPC_LIBRARY
    Boost::system
    Boost::log

    pthread
)

#-------------------------------------------------------------------------------
# Set SubDirectory
#-------------------------------------------------------------------------------
add_subdirectory(example)

# # -------------------------------------------------------------------------------
# # Set gTest
# # -------------------------------------------------------------------------------
# include(cmake/googletest.cmake)

# fetch_googletest(
#     ${PROJECT_SOURCE_DIR}/cmake
#     ${PROJECT_BINARY_DIR}/googletest
# )

# enable_testing()

# add_subdirectory(test)
