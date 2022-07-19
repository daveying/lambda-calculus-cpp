cmake_minimum_required(VERSION 3.14)

project(lambda-calculus C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
