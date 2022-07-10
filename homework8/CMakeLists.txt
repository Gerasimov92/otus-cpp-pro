cmake_minimum_required(VERSION 3.8)

project(homework8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS filesystem program_options regex REQUIRED)

add_executable(bayan
    main.cpp
    DuplicateFinder.cpp
    DuplicateFinder.h
)

target_compile_definitions(bayan PRIVATE BOOST_TEST_DYN_LINK BOOST_FILESYSTEM_NO_DEPRECATED)
target_include_directories(bayan PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(bayan ${Boost_LIBRARIES})
