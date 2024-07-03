//
// Created by AHMET ERDEM on 19.01.2024.
//

#pragma once

#ifndef VECTOR_CPP_VECTOR_H
#define VECTOR_CPP_VECTOR_H

#include <type_traits>
#include "./functions.h"

template <typename T>
class Vector {
protected:
    unsigned int size = 0;
    template <typename U>
    using ctype = typename std::common_type<T, U>::type;

public:
    unsigned int length = 0;
    T *data = nullptr;

    Vector();

    Vector(unsigned int data_size, T *data_array);

    [[nodiscard]] Vector<T> copy() const;

    void sort(const bool& reverse = false);

    unsigned int getsize();

    unsigned int getlength();

    void resize();

    T operator[] (int index) const;

    friend std::ostream& operator<< (std::ostream& o, const Vector& a) {
        o << "[";
        if (a.length == 0) {
            o << "]";
            return o;
        }
        int i;
        for (i = 0; i < a.length - 1; i++) {
            o << *(a.data + i) << ", ";
        }
        o << *(a.data + i) << "]";
        return o;
    }

    void append(const T& inn);

    T pop(int index = -1);

    void insert(int index, T val);

    Vector<T> clear();

    int __pivot(const double& limiter = 0.00001);

    Vector<T> operator+ (const T& a) const;

    Vector<T> operator+ (const Vector<T>& v) const;

    template <typename U>
    Vector<ctype<U>> operator+ (const Vector<U>& v) const;

    template <typename U>
    Vector<ctype<U>> operator+ (const U& a) const;

    friend Vector<T> operator+ (const T& a, const Vector<T>& v) {
        T temp[v.length];
        for (int i = 0; i < v.length; i++) {
            temp[i] = *(v.data + i) + a;
        }
        Vector<T> result(v.length, temp);
        return result;
    };

    // Extraneous some shit
    template <typename U>
    friend Vector<ctype<U>> operator+ (const U& a, const Vector<T>& v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = static_cast<ctype<U>>(*(v.data + i)) + val;
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
    };

    Vector<T> operator+= (const T& a);

    template <typename U>
    Vector<T> operator+= (const U& a);

    Vector<T> operator+= (const Vector<T>& v);

    template <typename U>
    Vector<T> operator+= (const Vector<U>& v);

    Vector<T> operator-() const;

    Vector<T> operator- (const T& a) const;

    Vector<T> operator- (const Vector<T>& v) const;

    template <typename U>
    Vector<ctype<U>> operator- (const Vector<U>& v) const;

    template <typename U>
    Vector<ctype<U>> operator- (const U& a) const;

    friend Vector<T> operator- (const T& a, const Vector<T>& v) {
        T temp[v.length];
        for (int i = 0; i < v.length; i++) {
            temp[i] = a - *(v.data + i);
        }
        Vector<T> result(v.length, temp);
        return result;
    };

    template <typename U>
    friend Vector<ctype<U>> operator- (const U& a, const Vector<T>& v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = val - static_cast<ctype<U>>(*(v.data + i));
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
    };

    Vector<T> operator-= (const T& a);

    template <typename U>
    Vector<T> operator-= (const U& a);

    Vector<T> operator-= (const Vector<T>& v);

    template <typename U>
    Vector<T> operator-= (const Vector<U>& v);

    Vector<T> operator* (const T& a) const;

    T operator* (const Vector<T>& v) const;

    template <typename U>
    ctype<U> operator* (const Vector<U>& v) const;

    friend Vector<T> operator* (const T& a, const Vector<T>& v) {
        T temp[v.length];
        for (int i = 0; i < v.length; i++) {
            temp[i] = *(v.data + i ) * a;
        }
        Vector<T> result(v.length, temp);
        return result;
    };

    template <typename U>
    Vector<ctype<U>> operator* (const U& a) const;

    template <typename U>
    friend Vector<ctype<U>> operator* (const U& a, const Vector<T>& v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = static_cast<ctype<U>>(*(v.data + i)) * val;
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
    };

    Vector<T> operator*= (const T& a);

    template <typename U>
    Vector<T> operator*= (const U& a);

    template <typename U>
    Vector<double> operator/ (const U& a) const;

    template <typename U>
    Vector<double> operator/ (const Vector<U>& v) const;

    Vector<T> operator++ ();

    Vector<T> operator-- ();

    bool operator== (const Vector<T>& v) const;

    template <typename U>
    bool operator== (const Vector<U>& v) const;

    bool operator!= (const Vector<T>& v) const;

    template <typename U>
    bool operator!= (const Vector<U>& v) const;

    bool operator> (const Vector<T>& v) const;

    template <typename U>
    bool operator> (const Vector<U>& v) const;

    bool operator>= (const Vector<T>& v) const;

    template <typename U>
    bool operator>= (const Vector<U>& v) const;

    bool operator< (const Vector<T>& v) const;

    template <typename U>
    bool operator< (const Vector<U>& v) const;

    bool operator<= (const Vector<T>& v) const;

    template <typename U>
    bool operator<= (const Vector<U>& v) const;

    template <typename U>
    Vector<bool> operator|| (const Vector<U>& v) const;

    template <typename U>
    Vector<bool> operator&& (const Vector<U>& v) const;

    template <typename U>
    Vector<bool> operator^ (const Vector<U>& v) const;

    [[nodiscard]] T dot (const Vector<T>& v) const;

    template <typename U>
    ctype<U> dot (const Vector<U>& v) const;

    void shift(unsigned int amount, const bool& direction = true);

    void swap(const unsigned int& first, const unsigned int& second);

    static Vector<T> zero(const unsigned int& dim);

    static Vector<T> one(const unsigned int& dim);

    Vector<double> unit();

    [[nodiscard]] double len() const;

    template <typename U>
    Vector<double> proj(const Vector<U>& v);

    static Vector<double>* spanify(const unsigned int& amount, Vector<double> array[]);

    static bool doesSpan(const unsigned int& amount, const Vector<double> array[], const double& limiter = 0.0000000001);

    static Vector<int> randVint(const int& d, const int& a, const int& b);

    static Vector<float> randVfloat(const int& d, const float& a, const float& b);

    static Vector<double> randVdouble(const int& d, const double& a, const double& b);

    static Vector<bool> randVbool(const int& d);

    [[nodiscard]] Vector<int> toInt() const;

    [[nodiscard]] Vector<float> toFloat() const;

    [[nodiscard]] Vector<double> toDouble() const;

    [[nodiscard]] Vector<bool> toBool() const;

    static double determinant(const int& amount, const Vector<T>* v_list);

    static Vector<double> cross(const int& amount, const Vector<T>* v_list);

    void reverse();

    Vector<T> cumsum() const;

    T sum() const;

    [[nodiscard]] Vector<double> softmax() const;

    [[nodiscard]] Vector<double> sig(const double& a, const unsigned int& resolution = 15) const;

    [[nodiscard]] Vector<double> relu(const double& cutoff = 0, const double& leak = 0) const;

    [[nodiscard]] Vector<double> minmax() const;

    template <typename U>
    Vector<U> map(std::function<U(T)> f);

    Vector<T> filter(std::function<bool(T)> f);
};

#endif //VECTOR_CPP_VECTOR_H
