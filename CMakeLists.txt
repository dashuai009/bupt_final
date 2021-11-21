cmake_minimum_required(VERSION 3.20)
project(bupt_final)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(bupt_final
        AC.h
        Buffer.h
        Huffman.h
        Log.h
        main.cpp)
set(CMAKE_CXX_COMPILER  "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "-Wall -fsanitize=undefined -O2")
