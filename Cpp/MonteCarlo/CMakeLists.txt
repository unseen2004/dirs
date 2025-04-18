cmake_minimum_required(VERSION 3.29)
project(MonteCarlo)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

add_executable(MonteCarlo main.cpp random.h matplotlibcpp.h
        matplotlibcpp.h)

target_link_libraries(MonteCarlo ${Python3_LIBRARIES})