cmake_minimum_required(VERSION 3.30)
project(lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/includes)

add_executable(exec1 src/z1/main.cpp)
add_executable(exec2 src/z2/main.cpp)
add_executable(exec3 src/z3/main.cpp)