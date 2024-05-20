//
// Created by AHMET ERDEM on 5.05.2024.
//

#include "../include/complex.h"
#include <ostream>

template <typename T>
complex<T>::complex(const T& real) {
    this->real = real;
}

template <typename T>
complex<T>::complex(const T& real, const T& imag) {
    this->real = real;
    this->imaginary = imag;
}

template <typename T>
std::ostream& operator<< (std::ostream& o, const complex<T>& c) {
    if (c.imaginary >= 0) {
        o << c.real << " + " << c.imaginary << "i";
        return o;
    }
    o << c.real << " - " << -c.imaginary << "i";
    return o;
}

template <typename T>
complex<T> complex<T>::operator+ (const T& val) const {
    return complex<T>(this->real + val, this->imaginary);
}

template <typename T>
complex<T> operator+ (const T& val, const complex<T>& c) {
    return complex<T>(c.real + val, c.imaginary);
}

template <typename T>
template <typename U>
complex<typename complex<T>::template ctype<U>> complex<T>::operator+ (const U& val) const {
    return complex<ctype<U>>(static_cast<ctype<U>>(this->real)
                             + static_cast<ctype<U>>(val), static_cast<ctype<U>>(this->imaginary));
}

template <typename T, typename U>
complex<typename complex<T>::template ctype<U>> operator+ (const U& val,const complex<T>& c){
    return complex<typename complex<T>::template ctype<U>>(static_cast<typename complex<T>::template ctype<U>>(c.real)
                             + static_cast<typename complex<T>::template ctype<U>>(val), static_cast<typename complex<T>::template ctype<U>>(c.imaginary));
}

template <typename T>
complex<T> complex<T>::operator+ (const complex<T>& c) const {
    return complex<T>(this->real + c.real, this->imaginary + c.imaginary);
}

template <typename T>
complex<T> operator+ (const complex<T>& c, const complex<T>& d) {
    return complex<T>(d.real + c.real, d.imaginary + c.imaginary);
}

template <typename T>
template <typename U>
complex<typename complex<T>::template ctype<U>> complex<T>::operator+ (const complex<U>& c) const {
    return complex<ctype<U>>(static_cast<ctype<U>>(this->real) + static_cast<ctype<U>>(c.real),
            static_cast<ctype<U>>(this->imaginary) + static_cast<ctype<U>>(c.imaginary));
}

template <typename T>
complex<T> complex<T>::operator+= (const T& a) {
    this->real += a;
    this->imaginary += a;
    return *this;
}

template <typename T>
template <typename U>
complex<T> complex<T>::operator+= (const U& a) {
    this->real += static_cast<T>(a);
    this->imaginary += static_cast<T>(a);
    return *this;
}

template <typename T>
complex<T> complex<T>::operator+= (const complex<T>& c) {
    this->real += c.real;
    this->imaginary += c.imaginary;
    return *this;
}

template <typename T>
template <typename U>
complex<T> complex<T>::operator+= (const complex<U>& c) {
    this->real += static_cast<T>(c.real);
    this->imaginary += static_cast<T>(c.imaginary);
    return *this;
}

template <typename T>
complex<T> complex<T>::operator- () const {
    return complex<T>(-this->real, -this->imaginary);
}

template <typename T>
complex<T> complex<T>::operator- (const T& val) const {
    return complex<T>(this->real - val, this->imaginary);
}

template <typename T>
complex<T> operator- (const T& val, const complex<T>& c) {
    return complex<T>(val - c.real, -c.imaginary);
}

template <typename T>
template <typename U>
complex<typename complex<T>::template ctype<U>> complex<T>::operator- (const U& val) const {
    return complex<ctype<U>>(static_cast<ctype<U>>(this->real) - static_cast<ctype<U>>(val),
            static_cast<ctype<U>>(this->imaginary));
}

template <typename T, typename U>
complex<typename complex<T>::template ctype<U>> operator- (const U& val, const complex<T>& c) {
    return complex<typename complex<T>::template ctype<U>>(static_cast<typename complex<T>::template ctype<U>>(val) - static_cast<typename complex<T>::template ctype<U>>(c.real),
            -static_cast<typename complex<T>::template ctype<U>>(c.imaginary));
}

template <typename T>
complex<T> complex<T>::operator- (const complex<T>& c) const {
    return complex<T>(this->real - c.real, this->imaginary - c.imaginary);
}

template <typename T>
template <typename U>
complex<typename complex<T>::template ctype<U>> complex<T>::operator- (const complex<U>& c) const {
    return complex<ctype<U>>(static_cast<ctype<U>>(this->real) - static_cast<ctype<U>>(c.real),
            static_cast<ctype<U>>(this->imaginary) - static_cast<ctype<U>>(c.imaginary));
}

template <typename T>
complex<T> complex<T>::operator-= (const T& a) {
    this->real -= a;
    this->imaginary -= a;
    return *this;
}

template <typename T>
template <typename U>
complex<T> complex<T>::operator-= (const U& a) {
    this->real -= static_cast<T>(a);
    this->imaginary -= static_cast<T>(a);
    return *this;
}

template <typename T>
complex<T> complex<T>::operator-= (const complex<T>& c) {
    this->real -= c.real;
    this->imaginary -= c.imaginary;
    return *this;
}

template <typename T>
template <typename U>
complex<T> complex<T>::operator-= (const complex<U>& c) {
    this->real -= static_cast<T>(c.real);
    this->imaginary -= static_cast<T>(c.imaginary);
    return *this;
}

template <typename T>
complex<T> complex<T>::operator* (const T& val) const {
    return complex<T>(this->real * val, this->imaginary * val);
}

template <typename T>
complex<T> operator* (const T& val, const complex<T>& c) {
    return complex<T>(val * c.real, val * c.imaginary);
}

template <typename T>
template <typename U>
complex<typename complex<T>::template ctype<U>> complex<T>::operator* (const U& val) const {
    return complex<ctype<U>>(static_cast<ctype<U>>(this->real) * static_cast<ctype<U>>(val),
            static_cast<ctype<U>>(this->imaginary) * static_cast<ctype<U>>(val));
}

template <typename T, typename U>
complex<typename complex<T>::template ctype<U>> operator* (const U& val, const complex<T>& c) {
    return complex<typename complex<T>::template ctype<U>>(static_cast<typename complex<T>::template ctype<U>>(val) * static_cast<typename complex<T>::template ctype<U>>(c.real),
            static_cast<typename complex<T>::template ctype<U>>(val) * static_cast<typename complex<T>::template ctype<U>>(c.imaginary));
}

template <typename T>
complex<T> complex<T>::operator* (const complex<T>& c) const {
    return complex<T>((this->real * c.real - this->imaginary * c.imaginary), 2 * this->imaginary * c.imaginary);
}

template <typename T>
template <typename U>
complex<typename complex<T>::template ctype<U>> complex<T>::operator* (const complex<U>& c) const {
    return complex<ctype<U>>((static_cast<ctype<U>>(this->real) * static_cast<ctype<U>>(c.real) -
                              static_cast<ctype<U>>(this->imaginary) * static_cast<ctype<U>>(c.imaginary)),
            2 * static_cast<ctype<U>>(this->imaginary) * static_cast<ctype<U>>(c.imaginary));
}

template <typename T>
complex<T> complex<T>::operator*= (const T& a) {
    this->real *= a;
    this->imaginary *= a;
    return *this;
}

template <typename T>
template <typename U>
complex<T> complex<T>::operator*= (const U& a) {
    this->real *= static_cast<T>(a);
    this->imaginary *= static_cast<T>(a);
    return *this;
}

template <typename T>
complex<T> complex<T>::operator*= (const complex<T>& c) {
    this->real = this->real * c.real + this->imaginary * c.imaginary;
    this->imaginary = 2 * this->imaginary * c.imaginary;
    return *this;
}

template <typename T>
template <typename U>
complex<T> complex<T>::operator*= (const complex<U>& c) {
    this->real = this->real * static_cast<T>(c.real) + this->imaginary * static_cast<T>(c.imaginary);
    this->imaginary = 2 * this->imaginary * static_cast<T>(c.imaginary);
    return *this;
}

template <typename T>
template <typename U>
complex<double> complex<T>::operator/ (const U& val) const {
    return complex<double>(this->real / val, this->imaginary / val);
}

template <typename T, typename U>
complex<double> operator/ (const U& val, const complex<T>& c) {
    auto temp = c.real * c.real - c.imaginary * c.imaginary;
    return complex<double>(val * c.real / temp, - c.imaginary * val / temp);
}

template <typename T>
template <typename U>
complex<double> complex<T>::operator/ (const complex<U>& c) const {
    auto val = this->real * c.real - this->imaginary * c.imaginary;
    return complex<double>((this->real * c.real + this->imaginary * c.imaginary) / val,
                           (c.real * this->imaginary - this->real * c.imaginary) / val);
}

template <typename T>
bool complex<T>::operator== (const complex<T>& c) const {
    return (this->real == c.real) and (this->imaginary == c.imaginary);
}

template <typename T>
template <typename U>
bool complex<T>::operator== (const complex<U>& c) const {
    return (this->real == c.real) and (this->imaginary == c.imaginary);
}

template <typename T>
bool complex<T>::operator!= (const complex<T>& c) const {
    return (this->real != c.real) and (this->imaginary != c.imaginary);
}

template <typename T>
template <typename U>
bool complex<T>::operator!= (const complex<U>& c) const {
    return (this->real != c.real) and (this->imaginary != c.imaginary);
}

template <typename T>
bool complex<T>::operator> (const complex<T>& c) const {
    return this->len() > c.len();
}

template <typename T>
template <typename U>
bool complex<T>::operator> (const complex<U>& c) const {
    return this->len() > c.len();
}

template <typename T>
bool complex<T>::operator>= (const complex<T>& c) const {
    return this->len() >= c.len();
}

template <typename T>
template <typename U>
bool complex<T>::operator>= (const complex<U>& c) const {
    return this->len() >= c.len();
}

template <typename T>
bool complex<T>::operator< (const complex<T>& c) const {
    return this->len() < c.len();
}

template <typename T>
template <typename U>
bool complex<T>::operator< (const complex<U>& c) const {
    return this->len() < c.len();
}

template <typename T>
bool complex<T>::operator<= (const complex<T>& c) const {
    return this->len() <= c.len();
}

template <typename T>
template <typename U>
bool complex<T>::operator<= (const complex<U>& c) const {
    return this->len() <= c.len();
}

template <typename T>
double complex<T>::len() const {
    return sqrt(this->real * this->real + this->imaginary * this->imaginary, 15);
}

template <typename T>
complex<T> complex<T>::conjugate() const {
    return complex<T>(this->real, -this->imaginary);
}

template <typename T>
complex<double> complex<T>::inverse() const {
    auto val = this->real * this->real - this->imaginary * this->imaginary;
    return complex<double>(this->real / val, -this->imaginary / val);
}

template <typename T>
complex<double> complex<T>::rotate(const double& angle, const unsigned int& resolution) const {
    return *this * complex<double>(cos(angle, resolution), sin(angle, resolution));
}

template <typename T>
complex<double> complex<T>::rotationFactor(const double& angle, const unsigned int& resolution) {
    return complex<double>(cos(angle, resolution), sin(angle, resolution));
}
