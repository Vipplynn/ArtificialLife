cmake_minimum_required(VERSION 3.16)
project(artificial)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

find_package(SDL3 REQUIRED CONFIG)

file(GLOB SOURCES "src/*.c")
add_executable(program ${SOURCES})
target_include_directories(program PRIVATE include)
target_link_libraries(program PRIVATE SDL3::SDL3)
