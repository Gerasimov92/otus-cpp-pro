cmake_minimum_required(VERSION 3.8)

project(otus-cpp-pro)

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

if(WITH_BOOST_TEST)
    enable_testing()
endif()

add_subdirectory(homework1)
