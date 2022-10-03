cmake_minimum_required(VERSION 3.21)
project(IFJ_Project_2022 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(IFJ_Project_2022
        src/scanner.c
        src/scanner.h)
