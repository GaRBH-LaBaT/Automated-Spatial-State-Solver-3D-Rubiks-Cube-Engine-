cmake_minimum_required(VERSION 3.17)
project(RubiksCubeSolver CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Strict warnings and extreme optimization for the IDA* hot loop
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /O2 /Ob2 /arch:AVX2 /fp:fast")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O3 -march=native -flto")
endif()

# Find all source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(solver ${SOURCES})

# Target include directories
target_include_directories(solver PRIVATE src)
