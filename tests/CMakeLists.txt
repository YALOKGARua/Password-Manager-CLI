cmake_minimum_required(VERSION 3.15)
project(PasswordManagerTests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_logger test_logger.cpp)
target_include_directories(test_logger PRIVATE
  ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(test_logger PRIVATE logger GTest::gtest_main)
add_test(NAME logger_tests COMMAND test_logger) 