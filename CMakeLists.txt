cmake_minimum_required(VERSION 3.20)
project(bupt_final)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(.)
include_directories(./data)
add_executable(bupt_final
        include/InputBuffer.h
        include/OutputBuffer.h
        include/AC.h
        include/BitSet.h
        include/CircularQueue.h
        include/CompressBuffer.h
        include/INFO.h
        include/Huffman.h
        include/Log.h
        include/SetOptions.h
        main.cpp)



enable_testing()
add_executable(
        huffman_test
        test/huffman_test.cpp
)
target_link_libraries(
        huffman_test
        gtest_main
)

add_executable(
        AC_test
        test/AC_test.cpp
)
target_link_libraries(
        AC_test
        gtest_main
)

set(CMAKE_CXX_COMPILER  "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "-Wall -O3 -lz")
include(GoogleTest)
gtest_discover_tests(huffman_test)
gtest_discover_tests(AC_test)
