PROJECT (test)

enable_testing()

SET ( TEST_SOURCES
  test_abstract_factory
  test_builder
)

foreach(TEST_NAME ${TEST_SOURCES})

  add_executable( ${TEST_NAME} ${TEST_NAME}.cpp )

  target_include_directories( ${TEST_NAME}
  PUBLIC
      ${CMAKE_SOURCE_DIR}/src
  )

  target_link_libraries( ${TEST_NAME} gtest gtest_main )

  add_test(
      NAME
        ${TEST_NAME}
      COMMAND
        ${TEST_NAME}
)

endforeach()