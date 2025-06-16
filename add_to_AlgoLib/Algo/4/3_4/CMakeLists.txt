cmake_minimum_required(VERSION 3.10)
project(RBBSTProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rb_demo
        main_rb_demo.cpp
        RBBST.cpp
)
target_include_directories(rb_demo PRIVATE .)

add_executable(rb_experiment
        main_rb_experiment.cpp
        RBBST.cpp
)
target_include_directories(rb_experiment PRIVATE .)
