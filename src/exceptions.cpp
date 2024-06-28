//
// Created by AHMET ERDEM on 5.05.2024.
//

#include "../include/exceptions.h"

IndexError::IndexError() = default;

const char* IndexError::what() const noexcept {
    return "IndexError: Incorrect indexing";
}

DimensionError::DimensionError() = default;

const char* DimensionError::what() const noexcept {
    return "DimensionError: Incorrect dimensions for the operation";
}

ZeroDivisionError::ZeroDivisionError() = default;

const char* ZeroDivisionError::what() const noexcept {
    return "ZeroDivisionError: Division by 0";
}

RangeError::RangeError() = default;

const char* RangeError::what() const noexcept {
    return "RangeError: Argument(s) out of range";
}

ArgumentError::ArgumentError() = default;

const char* ArgumentError::what() const noexcept {
    return "ArgumentError: Faulty configuration of argument(s)";
}
