cmake_minimum_required(VERSION 3.14)

project(lambda-calculus-cpp C CXX)

set(SOURCES
    meta.hpp
)

add_library(${PROJECT_NAME} INTERFACE ${SOURCES})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
