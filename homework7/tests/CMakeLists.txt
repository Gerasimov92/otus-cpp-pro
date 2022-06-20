cmake_minimum_required(VERSION 3.8)

project(tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(../)

add_executable(test_Parser
    tst_Parser.cpp
    ../Parser.cpp
    ../Parser.h
)
target_compile_options(test_Parser PUBLIC --coverage)
target_compile_definitions(test_Parser PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(test_Parser PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_Parser ${Boost_LIBRARIES} gcov)
add_test(NAME test_Parser COMMAND test_Parser)

add_executable(test_PrintVisitor
    tst_PrintVisitor.cpp
    ../PrintVisitor.cpp
    ../PrintVisitor.h
)
target_compile_options(test_PrintVisitor PUBLIC --coverage)
target_compile_definitions(test_PrintVisitor PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(test_PrintVisitor PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_PrintVisitor ${Boost_LIBRARIES} gcov)
add_test(NAME test_PrintVisitor COMMAND test_PrintVisitor)

add_executable(test_SaveToFileVisitor
    tst_SaveToFileVisitor.cpp
    ../SaveToFileVisitor.cpp
    ../SaveToFileVisitor.h
)
target_compile_options(test_SaveToFileVisitor PUBLIC --coverage)
target_compile_definitions(test_SaveToFileVisitor PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(test_SaveToFileVisitor PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_SaveToFileVisitor ${Boost_LIBRARIES} gcov)
add_test(NAME test_SaveToFileVisitor COMMAND test_SaveToFileVisitor)
