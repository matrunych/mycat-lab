cmake_minimum_required(VERSION 3.15)
project(mycat CXX)

set(CMAKE_CXX_STANDARD 17)

# Options
option(WARNINGS_AS_ERRORS   "Treat compiler warnings as errors."    OFF)
option(ENABLE_SANITIZERS    "Use sanitizers to detect errors."      OFF)
option(ENABLE_CONAN         "Use Conan as a package manager."       OFF)

# Include fixed CMake configuration
include(cmake/config.cmake)

# Project source compilation
include_directories(inc)
add_library(buffer_operations SHARED src/buffer_operations.cpp inc/buffer_operations.hpp)
add_executable(mycat src/main.cpp)
target_link_libraries(mycat buffer_operations)

# Add external packages
find_package(Boost 1.65.1 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(mycat Boost::program_options)

# Sanitizers configuration
if (ENABLE_SANITIZERS)
    set(SANITIZE_UNDEFINED ON)
    # Only one of Memory, Address, or Thread sanitizers is applicable at the time
    set(SANITIZE_MEMORY ON)
#    set(SANITIZE_ADDRESS ON)
#    set(SANITIZE_THREAD ON)
    find_package(Sanitizers)

    # CMake targets to which apply sanitizers
    add_sanitizers(buffer_operations add)
endif ()
