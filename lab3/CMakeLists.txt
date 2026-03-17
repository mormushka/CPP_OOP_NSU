cmake_minimum_required(VERSION 4.2.2)
project(Lab3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lab3 ./main.cpp)

target_include_directories(Lab3 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
