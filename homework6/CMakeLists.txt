cmake_minimum_required(VERSION 3.8)

project(homework6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matrix
    main.cpp
    Matrix.h
)
