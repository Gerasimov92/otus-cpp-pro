cmake_minimum_required(VERSION 3.8)

project(homework10 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)

add_executable(bulk_server
    main.cpp
    Server.cpp
    Server.h
    ../homework7/Command.h
    ../homework7/CommandBulk.h
    ../homework7/IVisitable.h
    ../homework7/IVisitor.h
    ../homework7/Parser.cpp
    ../homework7/Parser.h
    ../homework7/PrintVisitor.cpp
    ../homework7/PrintVisitor.h
    ../homework7/SaveToFileVisitor.cpp
    ../homework7/SaveToFileVisitor.h
)

target_include_directories(bulk_server PUBLIC
    ../homework7
)

target_compile_definitions(bulk_server PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(bulk_server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(bulk_server
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
