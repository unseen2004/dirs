cmake_minimum_required(VERSION 3.10)
project(BSTProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bst_demo
        main_demo.cpp
        BST.cpp
)

target_include_directories(bst_demo PRIVATE .)

add_executable(bst_experiment
        main_experiment.cpp
        BST.cpp
)

target_include_directories(bst_experiment PRIVATE .)
