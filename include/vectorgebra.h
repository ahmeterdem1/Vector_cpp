#ifndef VECTOR_CPP_VECTORGEBRA_H
#define VECTOR_CPP_VECTORGEBRA_H

#include "matrix.h"
#include "./complex.h"

template <typename T>
Matrix<T> reshape(const Vector<T>& v, const unsigned int& a, const unsigned int& b) {
    if (a * b != v.length) throw DimensionError();

    Vector<T> new_data[a];
    for (int i = 0; i < a; i++) {
        T temp[b];
        for (int j = 0; j < b; j++) {
            *(temp + j) = *(v.data + a * i + j);
        }
        *(new_data + i) = Vector<T>(b, temp);
    }
    Matrix<T> result(a, b, new_data);
    return result;
}

template <typename T>
Vector<T> reshape(const Matrix<T>& m, const unsigned int& a) {
    if (a != m.a * m.b) throw DimensionError();
    T temp[a];
    for (int i = 0; i < m.a; i++) {
        auto w = *(m.data->data + i);
        for (int j = 0; j < m.b; j++) {
            *(temp + m.a * i + j) = *(w->data + j);
        }
    }
    Vector<T> result(a, temp);
    return result;
}

template <typename T>
Matrix<T> reshape(const Matrix<T>& m, const unsigned int& a, const unsigned int& b) {
    if (a * b != m.a * m.b) throw DimensionError();
    Vector<T> new_data[a];
    for (int i = 0; i < a; i++) {
        T temp[b];
        for (int j = 0; j < b; j++) {
            unsigned int index = b * i + j;
            *(temp + j) = m[floor(index / m.a)][index % m.b];
        }
        *(new_data + i) = Vector<T>(b, temp);
    }
    Matrix<T> result(a, b, new_data);
    return result;
}

template <typename T>
Vector<complex<T>> conjugate(Vector<complex<T>> v) {
    if (v.length == 0) return v;
    for (int i = 0; i < v.length; i++) {
        *(v.data + i) = (*(v.data + i)).conjugate();
    }
    return v;
}

template <typename T>
Matrix<complex<T>> conjugate(Matrix<complex<T>> m) {
    if (m.a == 0) return m;
    for (int i = 0; i< m.a; i++) {
        auto v = *(m.data->data + i);
        for (int j = 0; j < m.b; j++) {
            *(v->data + j) = (*(v->data + j)).conjugate();
        }
    }
    return m;
}

template <typename T, typename U>
complex<double> frobeniousProduct(const Matrix<complex<T>>& m, const Matrix<complex<U>>& n) {
    if ((m.a != n.a) or (m.b != n.b)) throw DimensionError();
    if ((m.a == 0) or (n.a == 0)) throw RangeError();
    complex<double> sum;
    for (int i = 0; i < m.a; i++) {
        auto v = *(m.data->data + i);
        auto w = *(n.data->data + i);
        for (int j = 0; j < m.b; j++) {
            sum += (*(v->data + j)).conjugate() * (*(w->data + i)).conjugate();
        }
    }
    return sum;
}

template <typename T, typename U>
complex<double> frobeniousProduct(const Matrix<T>& m, const Matrix<complex<U>>& n) {
    if ((m.a != n.a) or (m.b != n.b)) throw DimensionError();
    if ((m.a == 0) or (n.a == 0)) throw RangeError();
    complex<double> sum;
    for (int i = 0; i < m.a; i++) {
        auto v = *(m.data->data + i);
        auto w = *(n.data->data + i);
        for (int j = 0; j < m.b; j++) {
            sum += *(v->data + j) * (*(w->data + i)).conjugate();
        }
    }
    return sum;
}

template <typename T, typename U>
double frobeniousProduct(const Matrix<T>& m, const Matrix<U>& n) {
    if ((m.a != n.a) or (m.b != n.b)) throw DimensionError();
    if ((m.a == 0) or (n.a == 0)) throw RangeError();
    double sum = 0;
    for (int i = 0; i < m.a; i++) {
        auto v = *(m.data->data + i);
        auto w = *(n.data->data + i);
        for (int j = 0; j < m.b; j++) {
            sum += *(v->data + j) * *(w->data + i);
        }
    }
    return sum;
}

typedef Vector<int> iVector;
typedef Vector<float> fVector;
typedef Vector<double> dVector;
typedef Vector<bool> bVector;
typedef Vector<long> lVector;
typedef Vector<short> sVector;

typedef Matrix<int> iMatrix;
typedef Matrix<float> fMatrix;
typedef Matrix<double> dMatrix;
typedef Matrix<bool> bMatrix;
typedef Matrix<long> lMatrix;
typedef Matrix<short> sMatrix;

typedef complex<int> icomplex;
typedef complex<float> fcomplex;
typedef complex<double> dcomplex;
typedef complex<bool> bcomplex;
typedef complex<long> lcompelx;
typedef complex<short> scomplex;

#endif //VECTOR_CPP_VECTORGEBRA_H