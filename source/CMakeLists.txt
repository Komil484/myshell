cmake_minimum_required(VERSION 3.10)

project(myshell VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES myutils.c myutils.h)

# main
add_executable(mysh main.c ${SOURCES})
