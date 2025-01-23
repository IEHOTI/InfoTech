cmake_minimum_required(VERSION 3.10)

project(Program2)

set(PROGRAM2_SOURCES src/main.cpp)

add_executable(Program2 ${PROGRAM2_SOURCES})

target_link_libraries(Program2 PRIVATE myLib pthread)
