cmake_minimum_required(VERSION 3.8)

project(homework12 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads)

add_executable(mapreduce
    main.cpp
    MapReduce.cpp
    MapReduce.h
)

target_link_libraries(mapreduce
    ${CMAKE_THREAD_LIBS_INIT}
)
