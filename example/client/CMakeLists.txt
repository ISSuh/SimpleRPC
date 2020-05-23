project(client)

message("SimpleRPC Client Examples")

include_directories(
    ${SRPC_INCLUDES}
)

#-------------------------------------------------------------------------------
# Project files
#-------------------------------------------------------------------------------
SET( SOURCES
    clientExample.cpp
)

#-------------------------------------------------------------------------------
# Final target
#-------------------------------------------------------------------------------
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${BUILD_DIR_EXAMPLES} )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${SRPC_LIBRARY} )
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "example")
