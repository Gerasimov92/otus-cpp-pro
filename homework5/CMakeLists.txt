cmake_minimum_required(VERSION 3.8)

project(homework5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(vector-graphics-editor
    main.cpp
    CompositeShape.cpp
    CompositeShape.h
    Document.cpp
    Document.h
    IController.h
    IObserver.h
    IRenderer.h
    IShape.h
    IView.h
    Line.cpp
    Line.h
    Observable.cpp
    Observable.h
    Point.h
    Rectangle.cpp
    Rectangle.h
)
