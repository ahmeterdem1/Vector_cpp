#ifndef VECTOR_CPP_VECTORGEBRA_H
#define VECTOR_CPP_VECTORGEBRA_H

#include "matrix.h"
#include "./complex.h"
#include <thread>

void __mul(Vector<double>** data1, const double* data2, const int id, unsigned int count, unsigned int stop, double* target) {
    double sum;
    Vector<double>* v;
    auto ptr = data1 + id * count;
    for (int i = 0; i < stop; i++) {
        sum = 0;
        // data1 is a vector pointer
        // data2 is a data pointer
        v = *(ptr + i);
        for (int j = 0; j < v->length; j++) {
            sum += *(v->data + j) * *(data2 + j);
        }
        *(target + id * count + i) = sum;
    }
}

Vector<double> matmul(const Matrix<double>& m, const Vector<double>& v) {
    if (m.b != v.length) throw DimensionError();
    auto *temp = new double[m.a];
    int t_count = int(floor(log2(m.a))) + 1; // Thread count
    std::thread *tlist[t_count]; // Thread list
    unsigned int size_tracker = m.a;
    const int line_count = int(floor(m.a / (t_count - 1))); // Amount of rows, "lines", each thread will get
    const double* target = v.data;
    Vector<double>** line = m.data->data;

    for (int i = 0; i < t_count; i++) {
        if (size_tracker < line_count) {
            auto *t = new std::thread(__mul, line, target, i, line_count, size_tracker, temp);
            *(tlist + i) = t;
        } else {
            auto *t = new std::thread(__mul, line, target, i, line_count, line_count, temp);
            *(tlist + i) = t;
            //(tlist + i)->start();
        }
        size_tracker -= line_count;
    }

    for (int i = 0; i < t_count; i++) {
        (*(tlist + i))->join();
    }
    for (int i = 0; i < t_count; i++) {
        delete *(tlist + i);
    }
    Vector<double> result(m.a, temp);
    delete[] temp;
    return result;
}

void __mulm(Vector<double>** data1, Vector<double>** data2, const int idx, const int idy,
            unsigned int countx, unsigned int county, unsigned int stopx,
            unsigned int stopy, unsigned int rowlength, Vector<double>* target) {

    auto y_factor = idy * county;
    Vector<double>* v;
    Vector<double>** ptrv = data1 + idx * countx;
    Vector<double>* xtarget = target + idx * countx; // Our starting row
    double sum;
    for (int i = 0; i < stopx; i++) {
        v = *(ptrv + i);
        for (int j = 0; j < stopy; j++) {
            // Do dot product here
            sum = 0;
            for (int k = 0; k < rowlength; k++) {
                sum += *(v->data + k) * *((*(data2 + k))->data + j);
            }
            *((xtarget + i)->data + y_factor + j) = sum;
        }
    }
}

Matrix<double> matmul(const Matrix<double>& m, const Matrix<double>& n) {
    if (m.b != n.a) throw DimensionError();
    auto *new_data = new Vector<double>[m.a]; //rows
    for (int i = 0; i < m.a; i++) {
        *(new_data + i) = Vector<double>::zero(n.b); // Fill the rows.
    }
    // Thread counts
    int x_count = int(floor(log2(m.a))) + 1;
    int y_count = int(floor(log2(n.b))) + 1;
    int t_count = x_count * y_count;
    std::thread *tlist[t_count];

    int x_lines = int(floor(m.a / (x_count - 1)));
    int y_lines = int(floor(n.b / (y_count - 1)));
    auto v = m.data->data;
    auto w = n.data->data;

    unsigned int x_size = m.a;
    unsigned int y_size = n.b;
    const unsigned int rowlength = m.b;

    for (int x = 0; x < x_count; x++) {
        y_size = n.b;
        if (x_size < x_lines) {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulm, v, w, x, y, x_lines, y_lines, x_size, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulm, v, w, x, y, x_lines, y_lines, x_size, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }

        } else {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulm, v, w, x, y, x_lines, y_lines, x_lines, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulm, v, w, x, y, x_lines, y_lines, x_lines, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }
        }
        x_size -= x_lines;
    }

    for (int i = 0; i < t_count; i++) {
        (*(tlist + i))->join();
    }
    Matrix<double> result(m.a, n.b, new_data);
    delete[] new_data;
    return result;
}

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

// Below function definitions only exist for the reason that
// named operations require different treatment for complex
// valued matrices and vectors. To keep the implementation
// consistent in itself, I carried all possible definitions
// of said operations here.
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
    Vector<T>* v;
    for (int i = 0; i < m.a; i++) {
        v = *(m.data->data + i);
        for (int j = 0; j < m.b; j++) {
            *(v->data + j) = (*(v->data + j)).conjugate();
        }
    }
    return m;
}

template <typename T>
Matrix<complex<T>> hconj(const Matrix<complex<T>>& m) {
    if (m.a == 0) return m;
    auto tpose = m.transpose();
    Vector<T>* v;
    for (int i = 0; i < tpose.a; i++) {
        v = *(tpose.data->data + i);
        for (int j = 0; j < tpose.b; j++) {
            *(v->data + j) = (*(v->data + j)).conjugate();
        }
    }
    return tpose;
}

template <typename T>
double norm(const Matrix<complex<T>>& m) {
    auto hpose = hconj(m);
    auto temp = hpose * m;
    double* eiglist = temp.eigenvalue();
    double result =  sqrt(maximum(temp.a, eiglist), 15);
    delete[] eiglist;
    temp.clear();
    hpose.clear();
    return result;
}

template <typename T>
double norm(const Matrix<T>& m) {
    auto tpose = m.transpose();
    auto temp = tpose * m;
    double* eiglist = temp.eigenvalue();
    double result =  sqrt(maximum(temp.a, eiglist), 15);
    delete[] eiglist;
    temp.clear();
    tpose.clear();
    return result;
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