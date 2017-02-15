cmake_minimum_required(VERSION 3.6)
project(Clion_test)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c function.c function.h)
add_executable(Clion_test ${SOURCE_FILES})