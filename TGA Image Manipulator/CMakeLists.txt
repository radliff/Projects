cmake_minimum_required(VERSION 3.26)
project(COP3503C)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(COP3503C
        src/Image.cpp
        src/Image.h
        src/main.cpp)
