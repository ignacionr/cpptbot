cmake_minimum_required(VERSION 3.10)

# Project name
project(cpptbot)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include current directory (where your header is located)
include_directories(${CMAKE_SOURCE_DIR})

# Recursively build the subdirectories
add_subdirectory(sample)
