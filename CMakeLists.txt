cmake_minimum_required(VERSION 3.16)
project(artificial)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# --- 1. Desktop Build (Linux/SDL3) ---
if(NOT CMAKE_CROSSCOMPILING)
    find_package(SDL3 REQUIRED CONFIG)
    
    # Path includes shared physics and the desktop main
    file(GLOB DESKTOP_SOURCES "src/physics/*.c" "src/desktop/*.c")
    
    add_executable(program_desktop ${DESKTOP_SOURCES})
    
    # Apply include path specifically to this target
    target_include_directories(program_desktop PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    target_link_libraries(program_desktop PRIVATE SDL3::SDL3 m)

# --- 2. Bare-Metal Build (ARM/QEMU) ---
else()
    # Path includes shared physics and bare-metal startup/main
    file(GLOB BAREMETAL_SOURCES "src/physics/*.c" "src/baremetal/*.c")
    
    add_executable(program_arm ${BAREMETAL_SOURCES})
    
    # Apply include path specifically to this target
    target_include_directories(program_arm PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
    
    # Link with the required linker script for the LM3S6965
    target_link_options(program_arm PRIVATE "-T${CMAKE_CURRENT_SOURCE_DIR}/src/baremetal/linker.ld")
    target_link_libraries(program_arm PRIVATE m)
endif()
