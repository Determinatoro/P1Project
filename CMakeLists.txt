cmake_minimum_required(VERSION 3.12)
project(P1Project C)

set(CMAKE_C_STANDARD 90)

add_executable(P1Project main.c gps_parser.c gps_parser.h csv_parser.c csv_parser.h file_helper.c file_helper.h helper.c helper.h file_selector.c file_selector.h)