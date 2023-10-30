cmake_minimum_required(VERSION 3.10)

# Project name
project(cpptbot_sample)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# External dependencies
include(FetchContent)

# Fetch and make cpr available
FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
)
FetchContent_MakeAvailable(cpr)

# Fetch and make nlohmann_json available
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Define your library target
add_library(cpptbot
    src/cpptbot.cpp
    include/cpptbot.hpp
)

# Link libraries
target_link_libraries(cpptbot PRIVATE cpr::cpr nlohmann_json::nlohmann_json)

# Include directories for the target
target_include_directories(cpptbot
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Add the sample directory to the build
add_subdirectory(sample)
