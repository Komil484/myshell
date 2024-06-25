cmake_minimum_required(VERSION 3.10)

project(myshell_tests VERSION 1.0)

# cmake vars
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CTEST_OUTPUT_ON_FAILURE ON)

# project vars
set(SOURCES ../source/myutils.c ../source/myutils.h)

set(MUNIT munit/munit.c munit/munit.h)


set(TESTS
    test_get_input.c
    test_get_input.h
    test_parse.c
    test_parse.h
)

add_executable(test_mysh
    test_main.c
    ${SOURCES}
    ${MUNIT}
    ${TESTS}
)

target_include_directories(test_mysh PRIVATE
    munit
    ../source
)

enable_testing()
add_test(NAME MySHTests
    COMMAND test_mysh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
