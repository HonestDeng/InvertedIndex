cmake_minimum_required(VERSION 3.28)
project(InvertedIndex)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC "src/*.cpp")
add_library(InvertedIndex STATIC ${SRC})

add_subdirectory(tests)



