cmake_minimum_required(VERSION 3.26)
project(ASCII_SSS C)

set(CMAKE_C_STANDARD 11)

add_executable(ASCII_SSS src/main.c
        src/vector.c
        src/vector.h)
