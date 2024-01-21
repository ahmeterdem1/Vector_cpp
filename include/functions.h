//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_FUNCTIONS_H
#define VECTOR_CPP_FUNCTIONS_H

#include <functional>
#include "helpers.h"

template <typename T, typename U>
using fp = std::function<T(U)>;

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

unsigned int factorial(const unsigned int& x) {
    unsigned int factor = 1;
    for (unsigned int i = x; i > 0; i--) {
        factor *= i;
    }
    return factor;
}

unsigned int permutation(const unsigned int& x, const unsigned int& y) {
    if (x < 1 or y < 1 or x < y) throw RangeError();
    unsigned int result = 1;
    for (unsigned int i = y + 1; i < x + 1; i++) {
        result *= i;
    }
    return result;
}

double combination(const unsigned int& x, const unsigned int& y) {
    if (x < 0 or y < 0 or x < y) throw RangeError();
    double result = 1;
    double count = 1;
    for (unsigned int i = y + 1; i < x + 1; i++) {
        result *= i / count;
        count += 1;
    }
    return result;
}

double multinomial(unsigned int n, const unsigned int& argc, const int *list) {
    int sum = 0;
    int val;
    int copy_list[argc];
    for (int i = 0; i < argc; i++) {
        val = *(list + i);
        if (val <= 0) throw RangeError();
        sum += val;
        *(copy_list + i) = val;
    }
    if (sum != n) throw RangeError();
    double result = 1;
    while (n != 1) {
        result *= n;
        for (int i = 0; i < argc; i++) {
            val = *(copy_list + i);
            if (val == 0) continue;
            result /= val;
            *(copy_list + i) -= 1;
        }
        n -= 1;
    }
    return result;
}

double binomial(const unsigned int& n, const unsigned int& k, const double& p) {
    if (p < 0 or p > 1) throw RangeError();
    return combination(n, k) * pow(p, k) * pow(1 - p, n - k);
}

double geometrical(const unsigned int& n, const double& p) {
    // Behaviour change from Vectorgebra/Python --> n == 0 case returns Undefined() for Python version
    if (p < 0 or p > 1 or n == 0) throw RangeError();
    return p * pow(1 - p, n);
}

double __cumdiv(const double& x, const unsigned int& power) {
    double factor = 1;
    for (unsigned int i = power; i > 0; i--) {
        factor *= x/i;
    }
    return factor;
}

double e(const double& x, const unsigned int& resolution = 15) {
    if (resolution == 0) throw RangeError();
    double sum = 1;
    for (int i = 1; i < resolution; i++) {
        sum += __cumdiv(x, i);
    }

    return sum;
}

double poisson(const unsigned int& k, const double& l) {
    if (l < 0 or k < 0) throw RangeError();
    return pow(l, k) * e(-l) / factorial(k);
}

double normal(const double& x, const unsigned int& resolution = 15) {
    return e(-pow(x, 2) / 2, resolution) / sqrt2PI;
}

double gaussian(const double& x, const double& mean = 0, const double& sigma = 1, const unsigned int& resolution = 15) {
    // Default case, which is non-existent in Vectorgebra/Python, is a normal distribution
    double coef = 1 / (sqrt2PI * sigma);
    double power = - pow(x - mean, 2) / (2 * pow(sigma, 2));
    return coef * e(power, resolution);
}

double laplace(const double& x, const double& sigma, const unsigned int& resolution = 15) {
    double coef = 1 / (sqrt2 * sigma);
    double power = - (sqrt2 / sigma) * abs(x);
    return coef * e(power, resolution);
}

double sin(const double& x, const unsigned int& resolution = 10) {
    if (resolution == 0) throw RangeError();
    double temp = (x - 360 * floor(x / 360)) * DtoR;
    double sum = 0;
    for (int i = 1; i < resolution; i++) {
        sum += pow(-1, i - 1) * __cumdiv(temp, 2 * i - 1);
    }
    return sum;
}

double cos(const double& x, const unsigned int& resolution = 10) {
    if (resolution == 0) throw RangeError();
    double temp = (x - 360 * floor(x / 360)) * DtoR;
    double sum = 1;
    for (int i = 1; i < resolution; i++) {
        sum += pow(-1, i) * __cumdiv(temp, 2 * i);
    }
    return sum;
}

double tan(const double& x, const unsigned int& resolution = 10) {
    if (resolution == 0) throw RangeError();
    double c = cos(x, resolution);
    if (c == 0) throw ZeroDivisionError();
    return sin(x, resolution) / c;
}

double cot(const double& x, const unsigned int& resolution = 10) {
    if (resolution == 0) throw RangeError();
    double s = sin(x, resolution);
    if (s == 0) throw ZeroDivisionError();
    return cos(x, resolution) / s;
}

double sinh(const double& x, const unsigned int& resolution = 15) {
    if (resolution == 0) throw RangeError();
    return (e(x, resolution) - e(-x, resolution)) / 2;
}

double cosh(const double& x, const unsigned int& resolution = 15) {
    if (resolution == 0) throw RangeError();
    return (e(x, resolution) + e(-x, resolution)) / 2;
}

double tanh(const double& x, const unsigned int& resolution = 15) {
    if (resolution == 0) throw RangeError();
    return sinh(x, resolution) / cosh(x, resolution); // cosh is never zero
}

double coth(const double& x, const unsigned int& resolution = 15) {
    if (resolution == 0) throw RangeError();
    auto s = sinh(x, resolution);
    if (s == 0) throw ZeroDivisionError(); // However, sinh can be zero
    return cosh(x, resolution) / s;
}

double arcsin(const double& x, const unsigned int& resolution = 15) {
    // For some reason, precision errors are high even if you increase the resolution
    if (resolution <= 1) throw RangeError();
    if (x > 1 or x < -1) throw RangeError();
    double c = 1;
    double sol = x;
    for (int i = 1; i < resolution; i++) {
        c *= 1 - 1 / (2 * i);
        sol += c * pow(x, 2 * i + 1) / (2 * i + 1);
    }
    return sol / DtoR;
}

double arccos(const double& x, const unsigned int& resolution = 15) {
    if (resolution <= 1) throw RangeError();
    if (x > 1 or x < -1) throw RangeError();
    return 90 - arcsin(x, resolution);
}

double log2(double x, const unsigned int& resolution = 15) {
    if (resolution <= 1) throw RangeError();
    if (x <= 0) throw RangeError();
    double count = 0;
    while (x > 2) {
        x /= 2;
        count++;
    }

    for (int i = 1; i < resolution; i++) {
        x = x * x;
        if (x >= 2) {
            count += 1 / pow(2, i);
            x /= 2;
        }
    }
    return count;
}

double ln(double x, const unsigned int& resolution = 15) {
    if (resolution <= 1) throw RangeError();
    if (x <= 0) throw RangeError();
    return log2(x, resolution) / log2E;
}

double log10(double x, const unsigned int& resolution = 15) {
    if (resolution <= 1) throw RangeError();
    if (x <= 0) throw RangeError();
    return log2(x, resolution) / log2_10;
}

double log(double x, double base = 2, const unsigned int& resolution = 15) {
    if (resolution <= 1) throw RangeError();
    if (x <= 0) throw RangeError();
    if (base <= 0 or base == 1) throw RangeError();
    return log2(x, resolution) / log2(base, resolution);
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
double derivative(fp<T, U> f, const double& a, const double& h = 0.0000000001) {
    return (f(a + h) - f(a)) / h;
}

template <typename T, typename U>
double integrate(fp<T, U> f, double a, double b, const double& delta = 0.01) {
    if (delta <= 0) throw RangeError();
    if (a == b) return 0;
    auto half = delta / 2;
    double sum = 0;

    while (a > b) {
        sum += f(b + half);
        b += delta;
    }

    while (b > a) {
        sum += f(a + half);
        a += delta;
    }
    return sum * delta;
}

template <typename T, typename U>
double findsol(fp<T, U> f, double x=0, const unsigned int& resolution = 15) {
    for (int i = 0; i < resolution; i++) {
        x = x - (f(x) / derivative(f, x));
    }
    return x;
}

template <typename T, typename U>
double Sum(fp<T, U> f, double a, const double& b, const double& step = 1, const bool& control = false, const double& limiter = 0.0000001) {
    if (b < a) throw RangeError();
    double sum = 0;
    if (control) {
        double deriv = limiter + 1;
        while (deriv > limiter) {
            sum += f(a);
            deriv = derivative<T, U>(f, a);
            a += step;
        }
        return sum;
    } else {
        while (b > a) {
            sum += f(a);
            a += step;
        }
        return sum;
    }
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
