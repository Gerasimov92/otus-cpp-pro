cmake_minimum_required(VERSION 3.8)

project(homework2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ip_filter
    main.cpp
    IPv4Address.cpp
    IPv4Address.h
    IPv4Filter.cpp
    IPv4Filter.h
)

if(WITH_BOOST_TEST)
    add_subdirectory(tests)
endif()
