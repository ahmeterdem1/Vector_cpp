cmake_minimum_required(VERSION 3.26)
project(Vector_cpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(Vector_cpp main.cpp
        include/vectorgebra.h
        include/vector.h
        include/exceptions.h
        include/functions.h
        include/helpers.h
        include/matrix.h
        include/complex.h
        include/node.h
        include/graph.h)
