cmake_minimum_required(VERSION 3.26)
project(Vector_cpp)

set(CMAKE_CXX_STANDARD 23)

add_executable(Vector_cpp main.cpp
        vectorgebra.h
        vector.h
        exceptions.h
        functions.h
        helpers.h
        matrix.h
        complex.h)
