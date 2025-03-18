cmake_minimum_required(VERSION 3.16)
project(TrafficSimulation)

# Set C++20 as required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find raylib package
find_package(raylib REQUIRED)

# Define source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add ALL src subdirectories to the include path globally
file(GLOB SUBDIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/src/*")
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${SUBDIR})
        target_include_directories(${PROJECT_NAME} PRIVATE ${SUBDIR})
    endif()
endforeach()

# Also include the main src directory
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)