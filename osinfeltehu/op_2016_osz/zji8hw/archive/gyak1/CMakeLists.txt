cmake_minimum_required(VERSION 3.6)
project(gyak1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    exec.c
    fcntl.c
    fcntl_0.c
    fork.c
    rand.c
    system.c)

add_executable(gyak1 ${SOURCE_FILES})