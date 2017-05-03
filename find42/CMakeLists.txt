cmake_minimum_required(VERSION 3.7)
project(find42)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        fringe.c
        fringe.h
        readme.txt
        search
        search.c
        state.h)

add_executable(find42 ${SOURCE_FILES})