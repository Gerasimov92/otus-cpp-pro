cmake_minimum_required(VERSION 3.8)

project(tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    file(COPY ip_filter.tsv DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

include_directories(../)

add_executable(test_IPv4Address
    tst_IPv4Address.cpp
    ../IPv4Address.cpp
    ../IPv4Address.h
)
target_compile_options(test_IPv4Address PUBLIC --coverage)
target_compile_definitions(test_IPv4Address PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(test_IPv4Address PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_IPv4Address ${Boost_LIBRARIES} gcov)
add_test(NAME test_IPv4Address COMMAND test_IPv4Address)

add_executable(test_IPv4Filter
    tst_IPv4Filter.cpp
    ../IPv4Address.cpp
    ../IPv4Address.h
    ../IPv4Filter.cpp
    ../IPv4Filter.h
)
target_compile_options(test_IPv4Filter PUBLIC --coverage)
target_compile_definitions(test_IPv4Filter PRIVATE BOOST_TEST_DYN_LINK)
target_include_directories(test_IPv4Filter PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(test_IPv4Filter ${Boost_LIBRARIES} gcov)
add_test(NAME test_IPv4Filter COMMAND test_IPv4Filter)
