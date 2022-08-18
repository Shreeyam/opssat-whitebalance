#cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_SYSTEM_PROCESSOR armv7)

SET(CMAKE_C_COMPILER "/usr/bin/arm-linux-gnueabihf-gcc-7")

SET(CMAKE_CXX_COMPILER "/usr/bin/arm-linux-gnueabihf-g++-7")

message(STATUS "using ${CMAKE_CXX_COMPILER}")

set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "using ${CMAKE_CXX_COMPILER}")
set(CMAKE_VERBOSE_MAKEFILE ON)

# set the project name
project(wb)

#set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Os -std=c++11")


# add includes
include_directories(
  include)


add_compile_options(-Wall -static -O0 -march=armv7-a -mfpu=neon-vfpv4 -funsafe-math-optimizations)

# add the executable
add_executable(wb src/main.cpp)


# target_link_libraries(opssat-segment.out
  # tensorflow-lite
# )
