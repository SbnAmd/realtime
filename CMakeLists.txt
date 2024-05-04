cmake_minimum_required(VERSION 3.26)
project(realtime C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(realtime
        scheduler.c
        scheduler.h)
add_link_options("-pthread")