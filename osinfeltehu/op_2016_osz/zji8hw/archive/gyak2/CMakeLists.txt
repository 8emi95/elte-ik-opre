cmake_minimum_required(VERSION 3.6)
project(gyak2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    setitimer.c
    sigaction.c
    sigaction_multiple.c
    sigaction_value.c
    signal.c
    sigprocmask.c test.c)

add_executable(gyak2 ${SOURCE_FILES})