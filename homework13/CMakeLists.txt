cmake_minimum_required(VERSION 3.8)

project(homework13 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fashion_mnist
    main.cpp
    Helpers.h
    Helpers.cpp
    IBinaryClassifier.h
    LogregClassifier.h
    LogregClassifier.cpp
)
