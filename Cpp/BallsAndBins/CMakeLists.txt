cmake_minimum_required(VERSION 3.29)
project(BallsAndBins)

set(CMAKE_CXX_STANDARD 23)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python3_INCLUDE_DIRS} ${Python3_NumPy_INCLUDE_DIRS})

add_executable(BallsAndBins main.cpp
        experiment.cpp
        experiment.h
        file_operations.cpp
        file_operations.h
        experiment2.cpp
        experiment2.h)

target_link_libraries(BallsAndBins ${Python3_LIBRARIES})
