cmake_minimum_required(VERSION 3.10)

project(Program1)

set(PROGRAM1_SOURCES src/main.cpp)

add_executable(Program1 ${PROGRAM1_SOURCES})

target_link_libraries(Program1 PRIVATE myLib pthread)

