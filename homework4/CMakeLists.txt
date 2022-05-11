cmake_minimum_required(VERSION 3.8)

project(homework4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(print_ip
    main.cpp
    print_ip.h
)
