cmake_minimum_required(VERSION 3.16)

project(Chip8 C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build SDL3
add_subdirectory(SDL3)

# Build our CHIP-8 emulator
add_executable(Chip8
    src/main.c
    src/chip8.c
)

# Link SDL3
target_link_libraries(Chip8 PRIVATE SDL3::SDL3)
