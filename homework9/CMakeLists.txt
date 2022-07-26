cmake_minimum_required(VERSION 3.8)

project(homework9 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads)

add_library(async
    async.cpp
    async.h
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

target_include_directories(async PUBLIC
    ../homework7
)

target_link_libraries(async
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable(bulk2
    main.cpp
)

target_link_libraries(bulk2
    async
)
