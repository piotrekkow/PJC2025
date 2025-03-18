cmake_minimum_required(VERSION 3.10)
project(TrafficSimulation)

set(CMAKE_CXX_STANDARD 20)

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