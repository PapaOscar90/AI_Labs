cmake_minimum_required(VERSION 3.7)
project(Nim)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
        "nim(1).c")

add_executable(Nim ${SOURCE_FILES})