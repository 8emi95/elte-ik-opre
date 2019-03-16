cmake_minimum_required(VERSION 3.6)
project(gyak0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    arg.c
    directory.c
    file.c
    mtime.c
    open_fopen.c
    passwd.c
    strings.c)

add_executable(gyak0 ${SOURCE_FILES})