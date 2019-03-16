cmake_minimum_required(VERSION 3.6)
project(gyak3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    nevcso.c
    pipe.c
    poll_cso.c
    ppoll.cso.c
    sel_cso.c test.c)

add_executable(gyak3 ${SOURCE_FILES})