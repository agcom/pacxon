cmake_minimum_required(VERSION 3.22.1)
project(PacXon C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(PacXon main.c)
target_link_libraries(PacXon curses)