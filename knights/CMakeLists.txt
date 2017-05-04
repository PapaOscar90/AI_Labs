cmake_minimum_required(VERSION 3.7)
project(knights)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        idknight.c)

add_executable(knights ${SOURCE_FILES})