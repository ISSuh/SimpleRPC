project(example)

message("SimpleRPC Examples")

SET ( SUBDIRS
  client
  server
)

foreach(SUBDIR ${SUBDIRS})
  add_subdirectory(${SUBDIR})
endforeach()

