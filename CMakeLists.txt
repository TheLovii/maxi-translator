cmake_minimum_required(VERSION 3.23)
project(maxi-translator
    VERSION 0.0.1
    DESCRIPTION "Simple and beautiful translator for programm language"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(analyzers/syntax)
add_subdirectory(analyzers/lexical)
add_executable(main main.cpp)

target_link_libraries(main PUBLIC lexical syntax)
