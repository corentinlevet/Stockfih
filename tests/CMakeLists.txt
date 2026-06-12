include(GoogleTest)

add_executable(stockfih_tests
  version_test.cpp
  square_test.cpp
  piece_test.cpp
  board_test.cpp
  fen_test.cpp
  movegen_test.cpp
)

target_link_libraries(stockfih_tests
  PRIVATE stockfih_engine GTest::gtest_main)

# Register each TEST() with CTest so `ctest` runs them individually.
gtest_discover_tests(stockfih_tests)
