cmake_minimum_required(VERSION 3.16)
project(TrafficSimulation)

# Set C++20 as required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find raylib package
find_package(raylib REQUIRED)

# Create executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/graph
    ${CMAKE_SOURCE_DIR}/src/vehicles
)

# Link libraries
target_link_libraries(${PROJECT_NAME} raylib)