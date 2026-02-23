cmake_minimum_required(VERSION 3.14)
project(PriorityQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(priority_queue_tests
    tests.cpp
)

target_include_directories(priority_queue_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(priority_queue_tests
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(priority_queue_tests)
