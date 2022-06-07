cmake_minimum_required(VERSION 3.8)

project(homework7 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bulk
    main.cpp
    Command.h
    CommandBulk.h
    IVisitable.h
    IVisitor.h
    Parser.cpp
    Parser.h
    PrintVisitor.cpp
    PrintVisitor.h
    SaveToFileVisitor.cpp
    SaveToFileVisitor.h
)

if(WITH_BOOST_TEST)
    add_subdirectory(tests)
endif()
