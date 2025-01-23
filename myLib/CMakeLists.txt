cmake_minimum_required(VERSION 3.10)

project(myLib)

set(LIBRARY_SOURCES src/myLib.cpp)
set(LIBRARY_HEADERS include/myLib.hpp)

add_library(myLib SHARED ${LIBRARY_SOURCES})

target_include_directories(myLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

