# CMakeLists.txt for the sample application

# Define the executable
add_executable(sample_app main.cpp)

# Link your library to the executable
target_link_libraries(sample_app cpptbot)

# Set the C++ standard for the sample application
set_target_properties(sample_app PROPERTIES CXX_STANDARD 20)

# Specify the include directory for nlohmann_json
target_include_directories(sample_app PRIVATE ${CMAKE_BINARY_DIR}/_deps/json-src/include)
target_include_directories(sample_app PRIVATE ${CMAKE_BINARY_DIR}/_deps/cpr-build/cpr_generated_includes)
target_include_directories(sample_app PRIVATE ${CMAKE_BINARY_DIR}/_deps/cpr-src/include)

target_link_libraries(sample_app cpr::cpr nlohmann_json::nlohmann_json)