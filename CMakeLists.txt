cmake_minimum_required(VERSION 3.8)

project(otus-cpp-pro)

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

if(WITH_BOOST_TEST)
    enable_testing()
endif()

add_subdirectory(docs)
add_subdirectory(homework1)
add_subdirectory(homework2)
add_subdirectory(homework3)
add_subdirectory(homework4)
add_subdirectory(homework5)
add_subdirectory(homework6)
add_subdirectory(homework7)
add_subdirectory(homework8)
