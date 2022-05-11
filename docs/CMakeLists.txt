cmake_minimum_required(VERSION 3.9)

project(docs)

find_package(Doxygen REQUIRED dot)

set(DOXYGEN_PROJECT_NAME "otus-cpp-pro")
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_HAVE_DOT YES)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_STRIP_FROM_PATH ../)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)

doxygen_add_docs(doxygen ALL
    ../
    COMMENT "Generate html docs"
)
