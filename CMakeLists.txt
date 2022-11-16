cmake_minimum_required(VERSION 3.16)
project(IFJ_Project_2022 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(IFJ_Project_2022
         src/main.c src/errors.h src/expr.c src/expr.h src/ial_hash.c src/ial_hash.h src/parser.c src/scanner.c src/scanner.h src/strings.h src/strings.c src/errors.c src/parser.h src/list.c src/list.h)