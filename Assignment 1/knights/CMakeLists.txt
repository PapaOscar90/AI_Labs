cmake_minimum_required(VERSION 3.7)
project(knights)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11 -Wall")

set(SOURCE_FILES
        knightAstar.c)

add_executable(knights ${SOURCE_FILES})