//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_COMPLEX_H
#define VECTOR_CPP_COMPLEX_H

#include "functions.h"
#include <ostream>

template <typename T>
class complex {
    template <typename U>
    using ctype = typename std::common_type<T, U>::type;

public:
    T real = 0;
    T imaginary = 0;

    complex() = default;

    explicit complex(const T& real);

    explicit complex(const T& real, const T& imag);

    friend std::ostream& operator<< (std::ostream& o, const complex& c);

    complex<T> operator+ (const T& val) const;

    friend complex<T> operator+ (const T& val, const complex<T>& c);

    template <typename U>
    complex<ctype<U>> operator+ (const U& val) const;

    template <typename U>
    friend complex<ctype<U>> operator+ (const U& val,const complex<T>& c);

    complex<T> operator+ (const complex<T>& c) const;

    friend complex<T> operator+ (const complex<T>& c, const complex<T>& d);

    template <typename U>
    complex<ctype<U>> operator+ (const complex<U>& c) const;

    complex<T> operator+= (const T& a);

    template <typename U>
    complex<T> operator+= (const U& a);

    complex<T> operator+= (const complex<T>& c);

    template <typename U>
    complex<T> operator+= (const complex<U>& c);

    complex<T> operator- () const;

    complex<T> operator- (const T& val) const;

    friend complex<T> operator- (const T& val, const complex<T>& c);

    template <typename U>
    complex<ctype<U>> operator- (const U& val) const;

    template <typename U>
    friend complex<ctype<U>> operator- (const U& val, const complex<T>& c);

    complex<T> operator- (const complex<T>& c) const;

    template <typename U>
    complex<ctype<U>> operator- (const complex<U>& c) const;

    complex<T> operator-= (const T& a);

    template <typename U>
    complex<T> operator-= (const U& a);

    complex<T> operator-= (const complex<T>& c);

    template <typename U>
    complex<T> operator-= (const complex<U>& c);

    complex<T> operator* (const T& val) const;

    friend complex<T> operator* (const T& val, const complex<T>& c);

    template <typename U>
    complex<ctype<U>> operator* (const U& val) const;

    template <typename U>
    friend complex<ctype<U>> operator* (const U& val, const complex<T>& c);

    complex<T> operator* (const complex<T>& c) const;

    template <typename U>
    complex<ctype<U>> operator* (const complex<U>& c) const;

    complex<T> operator*= (const T& a);

    template <typename U>
    complex<T> operator*= (const U& a);

    complex<T> operator*= (const complex<T>& c);

    template <typename U>
    complex<T> operator*= (const complex<U>& c);

    template <typename U>
    complex<double> operator/ (const U& val) const;

    template <typename U>
    friend complex<double> operator/ (const U& val, const complex<T>& c);

    template <typename U>
    complex<double> operator/ (const complex<U>& c) const;

    bool operator== (const complex<T>& c) const;

    template <typename U>
    bool operator== (const complex<U>& c) const;

    bool operator!= (const complex<T>& c) const;

    template <typename U>
    bool operator!= (const complex<U>& c) const;

    bool operator> (const complex<T>& c) const;

    template <typename U>
    bool operator> (const complex<U>& c) const;

    bool operator>= (const complex<T>& c) const;

    template <typename U>
    bool operator>= (const complex<U>& c) const;

    bool operator< (const complex<T>& c) const;

    template <typename U>
    bool operator< (const complex<U>& c) const;

    bool operator<= (const complex<T>& c) const;

    template <typename U>
    bool operator<= (const complex<U>& c) const;

    double len() const;

    [[nodiscard]] complex<T> conjugate() const;

    complex<double> inverse() const;

    complex<double> rotate(const double& angle, const unsigned int& resolution = 10) const;

    static complex<double> rotationFactor(const double& angle, const unsigned int& resolution = 10);
};

#endif //VECTOR_CPP_COMPLEX_H
