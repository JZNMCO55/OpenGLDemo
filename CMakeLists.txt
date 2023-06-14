cmake_minimum_required(VERSION 3.8)

project("test")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /utf-8")

add_subdirectory("Project/demo")
