cmake_minimum_required(VERSION 3.21)

project(CUNQASIMULATOR VERSION 0.0.1 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Adding C++20 standard as required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++ version ${CXX_STANDARD} configured.")
message(STATUS "${CMAKE_VERSION}")
if(${CMAKE_VERSION} VERSION_EQUAL "3.27.6")
    cmake_policy(SET CMP0144 OLD)
endif()
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")

if(NOT TARGET nlohmann_json::nlohmann_json)
  find_package(nlohmann_json CONFIG QUIET)
endif()
find_package(Threads REQUIRED)
find_package(MPI REQUIRED)
#find_package(OpenMP REQUIRED)

add_subdirectory(src)
add_subdirectory(examples)