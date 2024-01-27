//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_EXCEPTIONS_H
#define VECTOR_CPP_EXCEPTIONS_H

#include <exception>

class IndexError : public std::exception {
public:
    IndexError() = default;

    const char* what() {
        return "IndexError: Incorrect indexing";
    }
};

class DimensionError : public std::exception {
public:
    DimensionError() = default;

    const char* what() {
        return "DimensionError: Incorrect dimensions for the operation";
    }
};

class ZeroDivisionError : public std::exception {
public:
    ZeroDivisionError() = default;

    const char* what() {
        return "ZeroDivisionError: Division by 0";
    }
};

class RangeError : public std::exception {
public:
    RangeError() = default;

    const char* what() {
        return "RangeError: Argument(s) out of range";
    }
};

class ArgumentError : public std::exception {
public:
    ArgumentError() = default;

    const char* what() {
        return "ArgumentError: Faulty configuration of argument(s)";
    }
};

#endif //VECTOR_CPP_EXCEPTIONS_H
