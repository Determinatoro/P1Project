cmake_minimum_required(VERSION 3.12)
project(P1Project C)

set(CMAKE_C_STANDARD 90)

add_executable(P1Project main.c csv_helper.c csv_helper.h file_helper.c file_helper.h helper.c helper.h file_selector.c file_selector.h gps_logger_helper.c gps_logger_helper.h)