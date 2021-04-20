cmake_minimum_required(VERSION 3.17)
project(Threaded-Encryption C)

set(CMAKE_C_STANDARD 99)

add_library(Threaded-Encryption src/main.c)

add_library(Include include/encrypt-module.c)