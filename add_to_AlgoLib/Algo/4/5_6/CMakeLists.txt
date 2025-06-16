cmake_minimum_required(VERSION 3.10)
project(SplayProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(splay_demo
    main_splay_demo.cpp
    Splay.cpp
)
target_include_directories(splay_demo PRIVATE .)

add_executable(splay_experiment
    main_splay_experiment.cpp
    Splay.cpp
)
target_include_directories(splay_experiment PRIVATE .)
