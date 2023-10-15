cmake_minimum_required(VERSION 3.10)

# Project name
project(cpptbot_sample)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add cpr and nlohmann_json as dependencies
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

# Executable
add_executable(cpptbot_sample main.cpp)

# Link libraries
target_link_libraries(cpptbot_sample PRIVATE cpr::cpr nlohmann_json::nlohmann_json)