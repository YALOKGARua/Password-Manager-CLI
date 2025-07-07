enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(test_logger test_logger.cpp)
target_link_libraries(test_logger PRIVATE logger gtest_main)
add_test(NAME logger_tests COMMAND test_logger) 