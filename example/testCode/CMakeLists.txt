project(testCode)

message("SimpleRPC Test code")

include_directories(
     ${SRPC_INCLUDES}
)

#-------------------------------------------------------------------------------
# Project files
#-------------------------------------------------------------------------------
SET( EXAMPLE_SRC
    message_test
)

#-------------------------------------------------------------------------------
# Final target
#-------------------------------------------------------------------------------
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${BUILD_DIR_EXAMPLES} )

foreach(EXAMPLE_NAME ${EXAMPLE_SRC})

  add_executable( ${EXAMPLE_NAME} ${EXAMPLE_NAME}.cpp )

  target_link_libraries( ${EXAMPLE_NAME} ${SRPC_LIBRARY} )

  set_property(TARGET ${EXAMPLE_NAME} PROPERTY FOLDER "example")
endforeach()