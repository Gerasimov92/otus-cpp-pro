cmake_minimum_required(VERSION 3.8)

project(homework11 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)

add_executable(join_server
    main.cpp
    Server.cpp
    Server.h
    SimpleDatabase.cpp
    SimpleDatabase.h
)

target_compile_definitions(join_server PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(join_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(join_server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
