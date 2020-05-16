project(example)

message("SimpleRPC Examples")

SET ( BUILD_DIR_EXAMPLES ${CMAKE_SOURCE_DIR}/build/bin)



include_directories(
        ${PROJECT_DIR}/include
        ${Boost_INCLUDE_DIR}
)

#-------------------------------------------------------------------------------
# Project files
#-------------------------------------------------------------------------------
SET( SOURCES
    main.cpp
)

#-------------------------------------------------------------------------------
# Final target
#-------------------------------------------------------------------------------
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${BUILD_DIR_EXAMPLES} )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} -lboost_thread )
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "example")
