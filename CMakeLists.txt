cmake_minimum_required(VERSION 3.20)
project(bupt_final)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(./data)
add_executable(bupt_final
        AC.h
        BitSet.h
        Buffer.h
        CircularQueue.h
        Huffman.h
        Log.h
        SetOptions.h
        main.cpp ReadAndWriteFile.h)
add_executable(huffman_test
        Huffman.h
        test/huffman_test.cpp)
set(CMAKE_CXX_COMPILER  "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "-Wall -fsanitize=undefined -O0 -lz")
