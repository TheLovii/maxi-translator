cmake_minimum_required(VERSION 3.23)
project(maxi_translator)

set(CMAKE_CXX_STANDARD 14)

add_executable(maxi_translator analyzers/syntax/syntax.cpp analyzers/lexical/lexical.cpp main.cpp)
