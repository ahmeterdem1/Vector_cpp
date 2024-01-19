//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_COMPLEX_H
#define VECTOR_CPP_COMPLEX_H

#include "functions.h"
#include "exceptions.h"
#include "helpers.h"
#include <ostream>

template <typename T>
class complex {
    template <typename U>
    using ctype = typename std::common_type<T, U>::type;

public:
    T real = 0;
    T imaginary = 0;

    complex() = default;

    explicit complex(const T& real) {
        this->real = real;
    }

    explicit complex(const T& real, const T& imag) {
        this->real = real;
        this->imaginary = imag;
    }

    friend std::ostream& operator<< (std::ostream& o, const complex& c) {
        if (c.imaginary >= 0) {
            o << c.real << " + " << c.imaginary << "i";
            return o;
        }
        o << c.real << " - " << -c.imaginary << "i";
        return o;
    }

    complex<T> operator+ (const T& val) const {
        return complex<T>(this->real + val, this->imaginary);
    }

    friend complex<T> operator+ (const T& val, const complex<T>& c) {
        return complex<T>(c.real + val, c.imaginary);
    }

    template <typename U>
    complex<ctype<U>> operator+ (const U& val) const {
        return complex<ctype<U>>(static_cast<ctype<U>>(this->real)
                                 + static_cast<ctype<U>>(val), static_cast<ctype<U>>(this->imaginary));
    }

    template <typename U>
    friend complex<ctype<U>> operator+ (const U& val,const complex<T>& c){
        return complex<ctype<U>>(static_cast<ctype<U>>(c.real)
                                 + static_cast<ctype<U>>(val), static_cast<ctype<U>>(c.imaginary));
    }

    complex<T> operator+ (const complex<T>& c) const {
        return complex<T>(this->real + c.real, this->imaginary + c.imaginary);
    }

    friend complex<T> operator+ (const complex<T>& c, const complex<T>& d) {
        return complex<T>(d.real + c.real, d.imaginary + c.imaginary);
    }

    template <typename U>
    complex<ctype<U>> operator+ (const complex<U>& c) const {
        return complex<ctype<U>>(static_cast<ctype<U>>(this->real) + static_cast<ctype<U>>(c.real),
                                 static_cast<ctype<U>>(this->imaginary) + static_cast<ctype<U>>(c.imaginary));
    }

    complex<T> operator+= (const T& a) {
        this->real += a;
        this->imaginary += a;
        return *this;
    }

    template <typename U>
    complex<T> operator+= (const U& a) {
        this->real += static_cast<T>(a);
        this->imaginary += static_cast<T>(a);
        return *this;
    }

    complex<T> operator+= (const complex<T>& c) {
        this->real += c.real;
        this->imaginary += c.imaginary;
        return *this;
    }

    template <typename U>
    complex<T> operator+= (const complex<U>& c) {
        this->real += static_cast<T>(c.real);
        this->imaginary += static_cast<T>(c.imaginary);
        return *this;
    }

    complex<T> operator- (const T& val) const {
        return complex<T>(this->real - val, this->imaginary);
    }

    friend complex<T> operator- (const T& val, const complex<T>& c) {
        return complex<T>(val - c.real, -c.imaginary);
    }

    template <typename U>
    complex<ctype<U>> operator- (const U& val) const {
        return complex<ctype<U>>(static_cast<ctype<U>>(this->real) - static_cast<ctype<U>>(val),
                                 static_cast<ctype<U>>(this->imaginary));
    }

    template <typename U>
    friend complex<ctype<U>> operator- (const U& val, const complex<T>& c) {
        return complex<ctype<U>>(static_cast<ctype<U>>(val) - static_cast<ctype<U>>(c.real),
                                 -static_cast<ctype<U>>(c.imaginary));
    }

    complex<T> operator- (const complex<T>& c) const {
        return complex<T>(this->real - c.real, this->imaginary - c.imaginary);
    }

    template <typename U>
    complex<ctype<U>> operator- (const complex<U>& c) const {
        return complex<ctype<U>>(static_cast<ctype<U>>(this->real) - static_cast<ctype<U>>(c.real),
                                 static_cast<ctype<U>>(this->imaginary) - static_cast<ctype<U>>(c.imaginary));
    }

    complex<T> operator-= (const T& a) {
        this->real -= a;
        this->imaginary -= a;
        return *this;
    }

    template <typename U>
    complex<T> operator-= (const U& a) {
        this->real -= static_cast<T>(a);
        this->imaginary -= static_cast<T>(a);
        return *this;
    }

    complex<T> operator-= (const complex<T>& c) {
        this->real -= c.real;
        this->imaginary -= c.imaginary;
        return *this;
    }

    template <typename U>
    complex<T> operator-= (const complex<U>& c) {
        this->real -= static_cast<T>(c.real);
        this->imaginary -= static_cast<T>(c.imaginary);
        return *this;
    }

    complex<T> operator* (const T& val) const {
        return complex<T>(this->real * val, this->imaginary * val);
    }

    friend complex<T> operator* (const T& val, const complex<T>& c) {
        return complex<T>(val * c.real, val * c.imaginary);
    }

    template <typename U>
    complex<ctype<U>> operator* (const U& val) const {
        return complex<ctype<U>>(static_cast<ctype<U>>(this->real) * static_cast<ctype<U>>(val),
                                 static_cast<ctype<U>>(this->imaginary) * static_cast<ctype<U>>(val));
    }

    template <typename U>
    friend complex<ctype<U>> operator* (const U& val, const complex<T>& c) {
        return complex<ctype<U>>(static_cast<ctype<U>>(val) * static_cast<ctype<U>>(c.real),
                                 static_cast<ctype<U>>(val) * static_cast<ctype<U>>(c.imaginary));
    }

    complex<T> operator* (const complex<T>& c) const {
        return complex<T>((this->real * c.real - this->imaginary * c.imaginary), 2 * this->imaginary * c.imaginary);
    }

    template <typename U>
    complex<ctype<U>> operator* (const complex<U>& c) const {
        return complex<ctype<U>>((static_cast<ctype<U>>(this->real) * static_cast<ctype<U>>(c.real) -
                                  static_cast<ctype<U>>(this->imaginary) * static_cast<ctype<U>>(c.imaginary)),
                                 2 * static_cast<ctype<U>>(this->imaginary) * static_cast<ctype<U>>(c.imaginary));
    }

    complex<T> operator*= (const T& a) {
        this->real *= a;
        this->imaginary *= a;
        return *this;
    }

    template <typename U>
    complex<T> operator*= (const U& a) {
        this->real *= static_cast<T>(a);
        this->imaginary *= static_cast<T>(a);
        return *this;
    }

    complex<T> operator*= (const complex<T>& c) {
        this->real = this->real * c.real + this->imaginary * c.imaginary;
        this->imaginary = 2 * this->imaginary * c.imaginary;
        return *this;
    }

    template <typename U>
    complex<T> operator*= (const complex<U>& c) {
        this->real = this->real * static_cast<T>(c.real) + this->imaginary * static_cast<T>(c.imaginary);
        this->imaginary = 2 * this->imaginary * static_cast<T>(c.imaginary);
        return *this;
    }

    template <typename U>
    complex<double> operator/ (const U& val) const {
        return complex<double>(this->real / val, this->imaginary / val);
    }

    template <typename U>
    friend complex<double> operator/ (const U& val, const complex<T>& c) {
        auto temp = c.real * c.real - c.imaginary * c.imaginary;
        return complex<double>(val * c.real / temp, - c.imaginary * val / temp);
    }

    template <typename U>
    complex<double> operator/ (const complex<U>& c) const {
        auto val = this->real * c.real - this->imaginary * c.imaginary;
        return complex<double>((this->real * c.real + this->imaginary * c.imaginary) / val,
                               (c.real * this->imaginary - this->real * c.imaginary) / val);
    }

    // cannot implement /= because of the required type conversions to self

    bool operator== (const complex<T>& c) const {
        return (this->real == c.real) and (this->imaginary == c.imaginary);
    }

    template <typename U>
    bool operator== (const complex<U>& c) const {
        return (this->real == c.real) and (this->imaginary == c.imaginary);
    }

    bool operator!= (const complex<T>& c) const {
        return (this->real != c.real) and (this->imaginary != c.imaginary);
    }

    template <typename U>
    bool operator!= (const complex<U>& c) const {
        return (this->real != c.real) and (this->imaginary != c.imaginary);
    }

    bool operator> (const complex<T>& c) const {
        return this->len() > c.len();
    }

    template <typename U>
    bool operator> (const complex<U>& c) const {
        return this->len() > c.len();
    }

    bool operator>= (const complex<T>& c) const {
        return this->len() >= c.len();
    }

    template <typename U>
    bool operator>= (const complex<U>& c) const {
        return this->len() >= c.len();
    }

    bool operator< (const complex<T>& c) const {
        return this->len() < c.len();
    }

    template <typename U>
    bool operator< (const complex<U>& c) const {
        return this->len() < c.len();
    }

    bool operator<= (const complex<T>& c) const {
        return this->len() <= c.len();
    }

    template <typename U>
    bool operator<= (const complex<U>& c) const {
        return this->len() <= c.len();
    }

    double len() {
        return sqrt(this->real * this->real + this->imaginary * this->imaginary, 15);
    }

    [[nodiscard]] complex<T> conjugate() const {
        return complex<T>(this->real, -this->imaginary);
    }

    complex<double> inverse() {
        auto val = this->real * this->real - this->imaginary * this->imaginary;
        return complex<double>(this->real / val, -this->imaginary / val);
    }

    complex<double> rotate(const double& angle, const unsigned int& resolution = 10) {
        return *this * complex<double>(cos(angle, resolution), sin(angle, resolution));
    }

    static complex<double> rotationFactor(const double& angle, const unsigned int& resolution = 10) {
        return complex<double>(cos(angle, resolution), sin(angle, resolution));
    }
};

#endif //VECTOR_CPP_COMPLEX_H
