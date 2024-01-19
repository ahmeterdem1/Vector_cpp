//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_FUNCTIONS_H
#define VECTOR_CPP_FUNCTIONS_H

#include <functional>
#include "exceptions.h"
#include "helpers.h"

float randf() {
    float first = rand();
    float second = rand();
    while (second < first) {
        second = rand();
    }
    return first / second;
}

int randint(const int& a, const int& b) {
    if (b < a) throw RangeError();
    auto a_f = float(a);
    auto b_f = float(b);

    if (a_f < 0) {
        float b_p = b_f - a_f;
        float val = randf() * b_p;
        while (val > b_p) {
            val = randf() * b_p;
        }
        return val + a_f;
    }
    float val = (randf() * b_f) + a_f;
    while (val > b_f) {
        val = (randf() * b_f) + a_f;
    }
    return int(val);
}

float randfloat(const float& a, const float& b) {
    if (b <= a) throw RangeError();
    if (a < 0) {
        float b_p = b - a;
        float val = randf() * b_p;
        while (val > b_p) {
            val = randf() * b;
        }
        return val + a;
    }
    float val = (randf() * b) + a;
    while (val > b) {
        val = (randf() * b) + a;
    }
    return val;
}

double randdouble(const double& a, const double& b) {
    if (b <= a) throw RangeError();
    if (a < 0) {
        double b_p = b - a;
        double val = randf() * b_p;
        while (val > b_p) {
            val = randf() * b;
        }
        return val + a;
    }
    double val = (randf() * b) + a;
    while (val > b) {
        val = (randf() * b) + a;
    }
    return val;
}

bool randbool() {
    return (rand() % 2) == 1;
}

template <typename T>
void swap(T* a, T* b) { // This will be used in future algorithms
    T temp = *a;
    *a = *b;
    *b = temp;
}

template <typename T>
double sqrt(const T& val, const unsigned int& resolution = 15) {
    if (resolution == 0) throw RangeError();
    double temp = 2;
    for (int i = 0; i < resolution; i++) {
        temp = 0.5 * (temp + val/temp);
    }
    return temp;
}

int factorial(const int& x) {
    int factor = 1;
    for (int i = x; i > 0; i--) {
        factor *= i;
    }
    return factor;
}

double __cumdiv(const double& x, const unsigned int& power) {
    double factor = 1;
    for (unsigned int i = power; i > 0; i--) {
        factor *= x/i;
    }
    return factor;
}

double e(const double& x, const unsigned int& resolution = 15) {
    double sum = 1;

    for (int i = 1; i < resolution; i++) {
        sum += __cumdiv(x, i);
    }

    return sum;
}

double sin(const double& x, const unsigned int& resolution = 10) {
    double temp = (x - 360 * floor(x / 360)) * DtoR;
    double sum = 0;
    for (int i = 1; i < resolution; i++) {
        sum += pow(-1, i - 1) * __cumdiv(temp, 2 * i - 1);
    }
    return sum;
}

double cos(const double& x, const unsigned int& resolution = 10) {
    double temp = (x - 360 * floor(x / 360)) * DtoR;
    double sum = 1;
    for (int i = 1; i < resolution; i++) {
        sum += pow(-1, i) * __cumdiv(temp, 2 * i);
    }
    return sum;
}

double tan(const double& x, const unsigned int& resolution = 10) {
    double c = cos(x, resolution);
    if (c == 0) throw ZeroDivisionError();
    return sin(x, resolution) / c;
}

double cot(const double& x, const unsigned int& resolution = 10) {
    double s = sin(x, resolution);
    if (s == 0) throw ZeroDivisionError();
    return cos(x, resolution) / s;
}

double sigmoid(const double& x, const double& a, const unsigned int& resolution = 15) {
    return 1 / (1 + e(-a*x, resolution));
}

double ReLU(const double& x, const double& cutoff = 0, const double& leak = 0) {
    if (x > cutoff) return x;
    if (x > 0) return cutoff;
    return leak * x;
}

template <typename T, typename U>
double derivative(std::function<T(U)> f, const double& a, const double& h = 0.0000000001) {
    return (f(a + h) - f(a)) / h;
}

template <typename T>
T maximum(const unsigned int& amount, T* array) {
    if (amount <= 0) throw RangeError();
    T maxima = *array;
    for (int i = 1; i < amount; i++) {
        if (*(array + i) > maxima) maxima = *(array + i);
    }
    return maxima;
}

template <typename T>
T minimum(const unsigned int& amount, T* array) {
    if (amount <= 0) throw RangeError();
    T minima = *array;
    for (int i = 1; i < amount; i++) {
        if (*(array + i) < minima) minima = *(array + i);
    }
    return minima;
}

#endif //VECTOR_CPP_FUNCTIONS_H
