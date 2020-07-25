project(example)

message("SimpleRPC Examples")

SET ( SUBDIRS
#   client
#   server
  testCode
)

foreach(SUBDIR ${SUBDIRS})
  add_subdirectory(${SUBDIR})
endforeach()

