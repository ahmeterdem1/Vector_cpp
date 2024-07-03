//
// Created by AHMET ERDEM on 19.01.2024.
//

#pragma once

#ifndef VECTOR_CPP_MATRIX_H
#define VECTOR_CPP_MATRIX_H

#include "./vector.h"

template <typename T>
class Matrix {

    template <typename U>
    using ctype = typename std::common_type<T, U>::type;

public:

    Vector<Vector<T>*> *data = nullptr;
    unsigned int a = 0;
    unsigned int b = 0;

    Matrix() = default;

    Matrix<T>(const unsigned int& a, const unsigned int& b, Vector<T>* v_list);

    friend std::ostream& operator<< (std::ostream& o, const Matrix<T>& m) {
        if (m.a == 0) {
            o << "[]";
            return o;
        }
        int i;
        Vector<T>* v;
        for (i = 0; i < m.a; i++) {
            v = *(m.data->data + i);
            o << "[";
            int j;
            for (j = 0; j < m.b - 1; j++) {
                o << *(v->data + j) << ", ";
            }
            o << *(v->data + j) << "]" << std::endl;
        }
        return o;
    };

    void append(const Vector<T>& v);

    Vector<T> pop(int index = -1);

    void insert(int index, const Vector<T>& v);

    void clear();

    [[nodiscard]] Matrix<T> copy() const;

    void sort(const bool& reverse = false);

    void resize();

    void reverse();

    unsigned int getsize() const;

    Vector<T> operator[] (int index) const;

    [[nodiscard]] Matrix<T> transpose() const;

    Matrix<T> operator+ (const T& val) const;

    friend Matrix<T> operator+ (const T& val, const Matrix<T>& m) {
        Vector<T> new_data[m.a];
        for (int i = 0; i < m.a; i++) {
            T temp[m.b];
            auto v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = *(v->data + j) + val;
            }
            Vector<T> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(m.a, m.b, new_data);
        for (int i = 0; i < m.a; i++) {
            (new_data + i)->clear();
        }
        return result;
    };

    template <typename U>
    Matrix<ctype<U>> operator+ (const U& val) const;

    template <typename U>
    friend Matrix<ctype<U>> operator+ (const U& val, const Matrix<T>& m) {
        Vector<ctype<U>> new_data[m.a];
        auto c = static_cast<ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < m.a; i++) {
            ctype<U> temp[m.b];
            v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) + c;
            }
            Vector<ctype<U>> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(m.a, m.b, new_data);
        for (int i = 0; i < m.a; i++) {
            (new_data + i)->clear();
        }
        return result;
    };

    Matrix<T> operator+ (const Matrix<T>& m) const;

    template <typename U>
    Matrix<ctype<U>> operator+ (const Matrix<U>& m) const;

    Matrix<T> operator+= (const T& val);

    template <typename U>
    Matrix<T> operator+= (const U& val);

    Matrix<T> operator+= (const Matrix<T>& m);

    template <typename U>
    Matrix<T> operator+= (const Matrix<U>& m);

    Matrix<T> operator- () const;

    Matrix<T> operator- (const T& val) const;

    friend Matrix<T> operator- (const T& val, const Matrix<T>& m) {
        Vector<T> new_data[m.a];
        Vector<T>* v;
        for (int i = 0; i < m.a; i++) {
            T temp[m.b];
            v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = val - *(v->data + j);
            }
            Vector<T> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(m.a, m.b, new_data);
        for (int i = 0; i < m.a; i++) {
            (new_data + i)->clear();
        }
        return result;
    };

    template <typename U>
    Matrix<ctype<U>> operator- (const U& val) const;

    template <typename U>
    friend Matrix<ctype<U>> operator- (const U& val, const Matrix<T>& m) {
        Vector<ctype<U>> new_data[m.a];
        auto c = static_cast<ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < m.a; i++) {
            typename Matrix<T>::template ctype<U> temp[m.b];
            v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = c - static_cast<ctype<U>>(*(*v->data + j));
            }
            Vector<ctype<U>> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(m.a, m.b, new_data);
        for (int i = 0; i < m.a; i++) {
            (new_data + i)->clear();
        }
        return result;
    };

    Matrix<T> operator- (const Matrix<T>& m) const;

    template <typename U>
    Matrix<ctype<U>> operator- (const Matrix<U>& m) const;

    Matrix<T> operator-= (const T& val);

    template <typename U>
    Matrix<T> operator-= (const U& val);

    Matrix<T> operator-= (const Matrix<T>& m);

    template <typename U>
    Matrix<T> operator-= (const Matrix<U>& m);

    Matrix<T> operator* (const T& val) const;

    friend Matrix<T> operator* (const T& val, const Matrix<T>& m) {
        Vector<T> new_data[m.a];
        Vector<T>* v;
        for (int i = 0; i < m.a; i++) {
            T temp[m.b];
            v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = val * *(v->data + j);
            }
            Vector<T> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(m.a, m.b, new_data);
        for (int i = 0; i < m.a; i++) {
            (new_data + i)->clear();
        }
        return result;
    };

    template <typename U>
    Matrix<ctype<U>> operator* (const U& val) const;

    template <typename U>
    friend Matrix<ctype<U>> operator* (const U& val, const Matrix<T>& m) {
        Vector<typename Matrix<T>::template ctype<U>> new_data[m.a];
        auto c = static_cast<typename Matrix<T>::template ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < m.a; i++) {
            typename Matrix<T>::template ctype<U> temp[m.b];
            v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                *(temp + j) = c * static_cast<typename Matrix<T>::template ctype<U>>(*(v->data + j));
            }
            Vector<typename Matrix<T>::template ctype<U>> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<typename Matrix<T>::template ctype<U>> result(m.a, m.b, new_data);
        for (int i = 0; i < m.a; i++) {
            (new_data + i)->clear();
        }
        return result;
    };

    Matrix<T> operator* (const Matrix<T>& m) const;

    template <typename U>
    Matrix<ctype<U>> operator* (const Matrix<U>& m) const;

    Vector<T> operator* (const Vector<T>& v) const;

    template <typename U>
    Vector<ctype<U>> operator* (const Vector<U>& v) const;

    Matrix<T> operator*= (const T& val);

    template <typename U>
    Matrix<T> operator*= (const U& val);

    Matrix<T> operator*= (const Matrix<T>& m);

    template <typename U>
    Matrix<double> operator/ (const U& val) const;

    bool operator== (const Matrix<T>& m) const;

    template <typename U>
    bool operator== (const Matrix<U>& m) const;

    bool operator!= (const Matrix<T>& m) const;

    template <typename U>
    bool operator!= (const Matrix<U>& m) const;

    template <typename U>
    Matrix<bool> operator|| (const Matrix<U>& m) const;

    template <typename U>
    Matrix<bool> operator&& (const Matrix<U>& m) const;

    template <typename U>
    Matrix<bool> operator^ (const Matrix<U>& m) const;

    [[nodiscard]] Matrix<int> toInt() const;

    [[nodiscard]] Matrix<float> toFloat() const;

    [[nodiscard]] Matrix<double> toDouble() const;

    [[nodiscard]] Matrix<bool> toBool() const;

    static Matrix<T> zero(const unsigned int& a, const unsigned int& b);

    static Matrix<T> one(const unsigned int& a, const unsigned int& b);

    static Matrix<T> identity(const unsigned int& d);

    static Matrix<int> randMint(const unsigned int& m, const unsigned int& n, const int& a, const int& b);

    static Matrix<float> randMfloat(const unsigned int& m, const unsigned int& n, const float& a, const float& b);

    static Matrix<double> randMdouble(const unsigned int& m, const unsigned int& n, const double& a, const double& b);

    static Matrix<bool> randMbool(const unsigned int&m, const unsigned int& n);

    [[nodiscard]] Matrix<T> cumsum() const;

    [[nodiscard]] T sum() const;

    [[nodiscard]] Matrix<T> getDiagonal() const;

    [[nodiscard]] Matrix<T> getUpper() const;

    [[nodiscard]] Matrix<T> getLower() const;

    template <typename U>
    Matrix<U> map(std::function<U(T)> f) const;

    [[nodiscard]] T mulDiagonal() const;

    [[nodiscard]] T trace() const;

    [[nodiscard]] int* __pivots() const;

    Matrix<double> echelon(const double& lowlimit = 0.0000000001, const double& highlimit = 1000000);

    Matrix<double> rrechelon(const double& lowlimit = 0.0000000000000000000001, const double& highlimit = 1000000000);

    double determinant(const std::string& method = "echelon", const double& lowlimit = 0.0000000001, const double& highlimit = 1000000);

    [[nodiscard]] Matrix<double> inverse(const std::string& method = "iterative", const unsigned int& resolution = 15,
                                         const double& lowlimit = 0.0000000001, const double& highlimit = 1000000) const;

    static Matrix<double> givens(const unsigned int& dim, const unsigned int& i, const unsigned int& j,
                                 const double& angle, const unsigned int& resolution = 15);

    template <typename U>
    Vector<ctype<U>> leastSquares(const Vector<U>& v, const std::string& method = "iterative", const unsigned int& resolution = 15,
                                const double& lowlimit = 0.0000000001, const double& highlimit = 1000000) const;

    Matrix<double>* QR(const double& limiter = 0.0000000001) const;

    [[nodiscard]] double* eigenvalue(const unsigned int& resolution = 15) const;

    template <typename U>
    static double frobeniusProduct(const Matrix<T>& m, const Matrix<U>& n);

    // Negative indexing is allowed here
    [[nodiscard]] Matrix<T> submatrix(int a, int b, int c, int d) const;
};

#endif //VECTOR_CPP_MATRIX_H
