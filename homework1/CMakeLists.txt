cmake_minimum_required(VERSION 3.8)

project(homework1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HELLOWORLD_SRC
    HelloWorld.h
    HelloWorld.cpp
)

add_library(helloworld ${HELLOWORLD_SRC})
target_compile_options(helloworld PRIVATE
    -Wall -Wextra -pedantic -Werror
)

add_executable(helloworld-cli
    main.cpp
)
target_compile_options(helloworld-cli PRIVATE
    -Wall -Wextra -pedantic -Werror
)

add_dependencies(helloworld-cli
    helloworld
)

target_link_libraries(helloworld-cli
    helloworld
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    add_library(helloworld-coverage ${HELLOWORLD_SRC})
    target_compile_options(helloworld-coverage PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld-coverage PUBLIC --coverage)

    add_executable(test_helloworld tst_HelloWorld.cpp)
    target_compile_options(test_helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )

    target_compile_definitions(test_helloworld PRIVATE
        BOOST_TEST_DYN_LINK
    )

    target_include_directories(test_helloworld PRIVATE
        ${Boost_INCLUDE_DIRS}
    )

    target_link_libraries(test_helloworld
        ${Boost_LIBRARIES}
        gcov
        helloworld-coverage
    )

    add_test(NAME test_helloworld COMMAND test_helloworld)
endif()
