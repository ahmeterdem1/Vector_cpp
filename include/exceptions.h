//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_EXCEPTIONS_H
#define VECTOR_CPP_EXCEPTIONS_H

#include <exception>

class IndexError : public std::exception {
public:
    IndexError();

    const char* what();
};

class DimensionError : public std::exception {
public:
    DimensionError();

    const char* what();
};

class ZeroDivisionError : public std::exception {
public:
    ZeroDivisionError();

    const char* what();
};

class RangeError : public std::exception {
public:
    RangeError();

    const char* what();
};

class ArgumentError : public std::exception {
public:
    ArgumentError();

    const char* what();
};

#endif //VECTOR_CPP_EXCEPTIONS_H
