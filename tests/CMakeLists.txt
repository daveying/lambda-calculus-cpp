cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
   googletest
   URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
   )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# =================================================
set(TEST_NAME Meta-test)
add_executable(${TEST_NAME}
    MetaTests.cpp
    )
target_link_libraries(${TEST_NAME} PUBLIC lambda-calculus-cpp gtest_main)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
