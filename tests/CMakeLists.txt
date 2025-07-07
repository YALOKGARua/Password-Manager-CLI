enable_testing()
include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(test_logger test_logger.cpp)
set_target_properties(test_logger PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
target_link_libraries(test_logger PRIVATE logger GTest::gtest_main)
add_test(NAME logger_tests COMMAND ${CMAKE_BINARY_DIR}/tests/test_logger) 