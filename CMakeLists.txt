cmake_minimum_required(VERSION 3.20)
project(bupt_final)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bupt_final
        AC.h
        Log.h
        Buffer.h
        main.cpp)
set(CMAKE_CXX_FLAGS "/usr/local/gcc/bin/g++")
set(CMAKE_CXX_FLAGS "-Wall -fsanitize=undefined -O3")
