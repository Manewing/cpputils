add_executable(test_tuple
  test_tuple.cpp
)
add_test(TEST_TUPLE test_tuple)
target_link_libraries(test_tuple
  utils
)

add_executable(test_table
  test_table.cpp
)
add_test(TEST_TABLE test_table)
target_link_libraries(test_table
  utils
)

add_executable(test_am
  test_am.cpp
)
add_test(TEST_AM test_am)
target_link_libraries(test_am
  utils
)

add_executable(test_cc
  test_cc.cpp
)
add_test(TEST_CC test_cc)
target_link_libraries(test_cc
  utils
)
