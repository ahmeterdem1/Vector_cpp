#pragma once

#ifndef VECTOR_CPP_VECTORGEBRA_H
#define VECTOR_CPP_VECTORGEBRA_H

#include "./graph.h"
#include "./complex.h"
#include <thread>

inline void __muld(Vector<double>** data1, const double* data2, const int id, unsigned int count, unsigned int stop, double* target) {
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

inline void __mulf(Vector<float>** data1, const float* data2, const int id, unsigned int count, unsigned int stop, float* target) {
    float sum;
    Vector<float>* v;
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

inline void __muli(Vector<int>** data1, const int* data2, const int id, unsigned int count, unsigned int stop, int* target) {
    int sum;
    Vector<int>* v;
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

inline void __mull(Vector<long>** data1, const long* data2, const int id, unsigned int count, unsigned int stop, long* target) {
    long sum;
    Vector<long>* v;
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

inline void __muls(Vector<short>** data1, const short* data2, const int id, unsigned int count, unsigned int stop, short* target) {
    short sum;
    Vector<short>* v;
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

inline void __mulb(Vector<bool>** data1, const bool* data2, const int id, unsigned int count, unsigned int stop, bool* target) {
    bool sum;
    Vector<bool>* v;
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

inline Vector<double> matmul(const Matrix<double>& m, const Vector<double>& v) {
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
            auto *t = new std::thread(__muld, line, target, i, line_count, size_tracker, temp);
            *(tlist + i) = t;
        } else {
            auto *t = new std::thread(__muld, line, target, i, line_count, line_count, temp);
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

inline Vector<float> matmul(const Matrix<float>& m, const Vector<float>& v) {
    if (m.b != v.length) throw DimensionError();
    auto *temp = new float[m.a];
    int t_count = int(floor(log2(m.a))) + 1; // Thread count
    std::thread *tlist[t_count]; // Thread list
    unsigned int size_tracker = m.a;
    const int line_count = int(floor(m.a / (t_count - 1))); // Amount of rows, "lines", each thread will get
    const float* target = v.data;
    Vector<float>** line = m.data->data;

    for (int i = 0; i < t_count; i++) {
        if (size_tracker < line_count) {
            auto *t = new std::thread(__mulf, line, target, i, line_count, size_tracker, temp);
            *(tlist + i) = t;
        } else {
            auto *t = new std::thread(__mulf, line, target, i, line_count, line_count, temp);
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
    Vector<float> result(m.a, temp);
    delete[] temp;
    return result;
}

inline Vector<int> matmul(const Matrix<int>& m, const Vector<int>& v) {
    if (m.b != v.length) throw DimensionError();
    auto *temp = new int[m.a];
    int t_count = int(floor(log2(m.a))) + 1; // Thread count
    std::thread *tlist[t_count]; // Thread list
    unsigned int size_tracker = m.a;
    const int line_count = int(floor(m.a / (t_count - 1))); // Amount of rows, "lines", each thread will get
    const int* target = v.data;
    Vector<int>** line = m.data->data;

    for (int i = 0; i < t_count; i++) {
        if (size_tracker < line_count) {
            auto *t = new std::thread(__muli, line, target, i, line_count, size_tracker, temp);
            *(tlist + i) = t;
        } else {
            auto *t = new std::thread(__muli, line, target, i, line_count, line_count, temp);
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
    Vector<int> result(m.a, temp);
    delete[] temp;
    return result;
}

inline Vector<long> matmul(const Matrix<long>& m, const Vector<long>& v) {
    if (m.b != v.length) throw DimensionError();
    auto *temp = new long[m.a];
    int t_count = int(floor(log2(m.a))) + 1; // Thread count
    std::thread *tlist[t_count]; // Thread list
    unsigned int size_tracker = m.a;
    const int line_count = int(floor(m.a / (t_count - 1))); // Amount of rows, "lines", each thread will get
    const long* target = v.data;
    Vector<long>** line = m.data->data;

    for (int i = 0; i < t_count; i++) {
        if (size_tracker < line_count) {
            auto *t = new std::thread(__mull, line, target, i, line_count, size_tracker, temp);
            *(tlist + i) = t;
        } else {
            auto *t = new std::thread(__mull, line, target, i, line_count, line_count, temp);
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
    Vector<long> result(m.a, temp);
    delete[] temp;
    return result;
}

inline Vector<short> matmul(const Matrix<short>& m, const Vector<short>& v) {
    if (m.b != v.length) throw DimensionError();
    auto *temp = new short[m.a];
    int t_count = int(floor(log2(m.a))) + 1; // Thread count
    std::thread *tlist[t_count]; // Thread list
    unsigned int size_tracker = m.a;
    const int line_count = int(floor(m.a / (t_count - 1))); // Amount of rows, "lines", each thread will get
    const short* target = v.data;
    Vector<short>** line = m.data->data;

    for (int i = 0; i < t_count; i++) {
        if (size_tracker < line_count) {
            auto *t = new std::thread(__muls, line, target, i, line_count, size_tracker, temp);
            *(tlist + i) = t;
        } else {
            auto *t = new std::thread(__muls, line, target, i, line_count, line_count, temp);
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
    Vector<short> result(m.a, temp);
    delete[] temp;
    return result;
}

inline Vector<bool> matmul(const Matrix<bool>& m, const Vector<bool>& v) {
    if (m.b != v.length) throw DimensionError();
    auto *temp = new bool[m.a];
    int t_count = int(floor(log2(m.a))) + 1; // Thread count
    std::thread *tlist[t_count]; // Thread list
    unsigned int size_tracker = m.a;
    const int line_count = int(floor(m.a / (t_count - 1))); // Amount of rows, "lines", each thread will get
    const bool* target = v.data;
    Vector<bool>** line = m.data->data;

    for (int i = 0; i < t_count; i++) {
        if (size_tracker < line_count) {
            auto *t = new std::thread(__mulb, line, target, i, line_count, size_tracker, temp);
            *(tlist + i) = t;
        } else {
            auto *t = new std::thread(__mulb, line, target, i, line_count, line_count, temp);
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
    Vector<bool> result(m.a, temp);
    delete[] temp;
    return result;
}

inline void __mulmd(Vector<double>** data1, Vector<double>** data2, const int idx, const int idy,
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

inline void __mulmf(Vector<float>** data1, Vector<float>** data2, const int idx, const int idy,
            unsigned int countx, unsigned int county, unsigned int stopx,
            unsigned int stopy, unsigned int rowlength, Vector<float>* target) {

    auto y_factor = idy * county;
    Vector<float>* v;
    Vector<float>** ptrv = data1 + idx * countx;
    Vector<float>* xtarget = target + idx * countx; // Our starting row
    float sum;
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

inline void __mulmi(Vector<int>** data1, Vector<int>** data2, const int idx, const int idy,
             unsigned int countx, unsigned int county, unsigned int stopx,
             unsigned int stopy, unsigned int rowlength, Vector<int>* target) {

    auto y_factor = idy * county;
    Vector<int>* v;
    Vector<int>** ptrv = data1 + idx * countx;
    Vector<int>* xtarget = target + idx * countx; // Our starting row
    int sum;
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

inline void __mulml(Vector<long>** data1, Vector<long>** data2, const int idx, const int idy,
             unsigned int countx, unsigned int county, unsigned int stopx,
             unsigned int stopy, unsigned int rowlength, Vector<long>* target) {

    auto y_factor = idy * county;
    Vector<long>* v;
    Vector<long>** ptrv = data1 + idx * countx;
    Vector<long>* xtarget = target + idx * countx; // Our starting row
    long sum;
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

inline void __mulms(Vector<short>** data1, Vector<short>** data2, const int idx, const int idy,
             unsigned int countx, unsigned int county, unsigned int stopx,
             unsigned int stopy, unsigned int rowlength, Vector<short>* target) {

    auto y_factor = idy * county;
    Vector<short>* v;
    Vector<short>** ptrv = data1 + idx * countx;
    Vector<short>* xtarget = target + idx * countx; // Our starting row
    short sum;
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

inline void __mulmb(Vector<bool>** data1, Vector<bool>** data2, const int idx, const int idy,
             unsigned int countx, unsigned int county, unsigned int stopx,
             unsigned int stopy, unsigned int rowlength, Vector<bool>* target) {

    auto y_factor = idy * county;
    Vector<bool>* v;
    Vector<bool>** ptrv = data1 + idx * countx;
    Vector<bool>* xtarget = target + idx * countx; // Our starting row
    short sum;
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

inline Matrix<double> matmul(const Matrix<double>& m, const Matrix<double>& n) {
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
                    auto *t = new std::thread(__mulmd, v, w, x, y, x_lines, y_lines, x_size, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmd, v, w, x, y, x_lines, y_lines, x_size, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }

        } else {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulmd, v, w, x, y, x_lines, y_lines, x_lines, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmd, v, w, x, y, x_lines, y_lines, x_lines, y_lines, rowlength, new_data);
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

inline Matrix<float> matmul(const Matrix<float>& m, const Matrix<float>& n) {
    if (m.b != n.a) throw DimensionError();
    auto *new_data = new Vector<float>[m.a]; //rows
    for (int i = 0; i < m.a; i++) {
        *(new_data + i) = Vector<float>::zero(n.b); // Fill the rows.
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
                    auto *t = new std::thread(__mulmf, v, w, x, y, x_lines, y_lines, x_size, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmf, v, w, x, y, x_lines, y_lines, x_size, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }

        } else {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulmf, v, w, x, y, x_lines, y_lines, x_lines, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmf, v, w, x, y, x_lines, y_lines, x_lines, y_lines, rowlength, new_data);
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
    Matrix<float> result(m.a, n.b, new_data);
    delete[] new_data;
    return result;
}

inline Matrix<int> matmul(const Matrix<int>& m, const Matrix<int>& n) {
    if (m.b != n.a) throw DimensionError();
    auto *new_data = new Vector<int>[m.a]; //rows
    for (int i = 0; i < m.a; i++) {
        *(new_data + i) = Vector<int>::zero(n.b); // Fill the rows.
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
                    auto *t = new std::thread(__mulmi, v, w, x, y, x_lines, y_lines, x_size, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmi, v, w, x, y, x_lines, y_lines, x_size, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }

        } else {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulmi, v, w, x, y, x_lines, y_lines, x_lines, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmi, v, w, x, y, x_lines, y_lines, x_lines, y_lines, rowlength, new_data);
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
    Matrix<int> result(m.a, n.b, new_data);
    delete[] new_data;
    return result;
}

inline Matrix<short> matmul(const Matrix<short>& m, const Matrix<short>& n) {
    if (m.b != n.a) throw DimensionError();
    auto *new_data = new Vector<short>[m.a]; //rows
    for (int i = 0; i < m.a; i++) {
        *(new_data + i) = Vector<short>::zero(n.b); // Fill the rows.
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
                    auto *t = new std::thread(__mulms, v, w, x, y, x_lines, y_lines, x_size, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulms, v, w, x, y, x_lines, y_lines, x_size, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }

        } else {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulms, v, w, x, y, x_lines, y_lines, x_lines, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulms, v, w, x, y, x_lines, y_lines, x_lines, y_lines, rowlength, new_data);
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
    Matrix<short> result(m.a, n.b, new_data);
    delete[] new_data;
    return result;
}

inline Matrix<long> matmul(const Matrix<long>& m, const Matrix<long>& n) {
    if (m.b != n.a) throw DimensionError();
    auto *new_data = new Vector<long>[m.a]; //rows
    for (int i = 0; i < m.a; i++) {
        *(new_data + i) = Vector<long>::zero(n.b); // Fill the rows.
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
                    auto *t = new std::thread(__mulml, v, w, x, y, x_lines, y_lines, x_size, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulml, v, w, x, y, x_lines, y_lines, x_size, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }

        } else {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulml, v, w, x, y, x_lines, y_lines, x_lines, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulml, v, w, x, y, x_lines, y_lines, x_lines, y_lines, rowlength, new_data);
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
    Matrix<long> result(m.a, n.b, new_data);
    delete[] new_data;
    return result;
}

inline Matrix<bool> matmul(const Matrix<bool>& m, const Matrix<bool>& n) {
    if (m.b != n.a) throw DimensionError();
    auto *new_data = new Vector<bool>[m.a]; //rows
    for (int i = 0; i < m.a; i++) {
        *(new_data + i) = Vector<bool>::zero(n.b); // Fill the rows.
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
                    auto *t = new std::thread(__mulmb, v, w, x, y, x_lines, y_lines, x_size, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmb, v, w, x, y, x_lines, y_lines, x_size, y_lines, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                }
                y_size -= y_lines;
            }

        } else {
            for (int y = 0; y < y_count; y++) {
                if (y_size < y_lines) {
                    auto *t = new std::thread(__mulmb, v, w, x, y, x_lines, y_lines, x_lines, y_size, rowlength, new_data);
                    *(tlist + x * y_count + y) = t;
                } else {
                    auto *t = new std::thread(__mulmb, v, w, x, y, x_lines, y_lines, x_lines, y_lines, rowlength, new_data);
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
    Matrix<bool> result(m.a, n.b, new_data);
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

// Will .append() to the target if found, thread lock is needed
// We don't need any thread id's since we have mutex and vectors
inline void __find(std::function<double(double)> f, std::mutex* lock, double low,
            double high, double search_step, double resolution, Vector<double>* target) {

    bool temp_sign;
    bool last_sign = (f(low) >= 0);
    double value;

    for (double x: Range<double>(low, high, search_step)) {
        value = f(x);
        temp_sign = (value >= 0);
        if (temp_sign ^ last_sign) {
            while (true) {
                if (lock->try_lock()) {
                    target->append(x - (search_step / 2));
                    lock->unlock();
                    return;
                }
            }
        } else if (abs(value) < resolution) {
            while (true) {
                if (lock->try_lock()) {
                    target->append(x);
                    lock->unlock();
                    return;
                }
            }
        }
    }
}

// Will return a vector of found zeroes
// Since the function f will be looped over
// in a Range<double> loop, it must accept "double" and return "double"
inline Vector<double>* solve(std::function<double(double)> f, double low = -50, double high = 50,
                      double search_step = 0.1, double resolution = 0.01) {

    if (high <= low or search_step <= 0 or resolution <= 0 or resolution >= 1) throw RangeError();

    Vector<std::thread*> tlist; // yes, it is what you see
    auto zeroes = new Vector<double>;
    auto *threadlock = new std::mutex;

    bool temp_sign;
    bool last_sign = (f(low) >= 0);
    double value;
    double resf = sqrt(resolution, 15); // Effective resolution


    // I have to use custom range objects here
    for (auto x : Range<double>(low, high, search_step)) {
        value = f(x);
        temp_sign = (value >= 0);

        // If prev and incident signs are the same, no crossing occurred.
        // But the |value| may be very close to 0, which may indicate a zero (like at x=0 for x^2)
        if (!(temp_sign ^ last_sign) and (abs(value) < resf)) {
            auto t = new std::thread(__find, f, threadlock, x - search_step, x, search_step / 10, resolution, zeroes);
            tlist.append(t);

        } else if (temp_sign ^ last_sign) { // If the signs are different
            auto t = new std::thread(__find, f, threadlock, x - search_step, x, search_step / 10, resolution, zeroes);
            tlist.append(t);
        }
        last_sign = temp_sign;
    }

    // Both wait for the threads and clean the memory at the same loop
    for (int i = 0; i < tlist.length; i++) {
        (*(tlist.data + i))->join();
        delete *(tlist.data + i);
    }
    delete threadlock;
    tlist.clear();
    return zeroes;
}

template <typename T>
double* linearFit(const T* x, const T* y, const unsigned int& amount, const unsigned int& iterations = 15, const double& rate = 0.01) {
    if (iterations == 0 or amount == 0) throw RangeError();

    double b0 = 1;
    double b1 = 1;
    double sum1;
    double sum2;
    auto results = new double[2];
    for (int k = 0; k < iterations; k++) {
        sum1 = 0;
        sum2 = 0;
        for (int i = 0; i < amount; i++) {
            sum1 += (*(y + i) - b0 - b1 * *(x + i));
            sum2 += (*(y + i) - b0 - b1 * *(x + i)) * *(x + i);
        }
        b0 = b0 - rate * (-2 * sum1 / amount);
        b1 = b1 - rate * (-2 * sum2 / amount);
    }

    results[0] = b0;
    results[1] = b1;
    return results;
}

template <typename T>
double* linearFit(const Vector<T>& x, const Vector<T>& y, const unsigned int& iterations = 15, const double& rate = 0.01) {
    unsigned int N = x.length;
    if (iterations == 0 or N == 0 or y.length == 0) throw RangeError();
    if (N != y.length) throw DimensionError();

    double b0 = 1;
    double b1 = 1;
    double sum1;
    double sum2;
    auto results = new double[2];

    for (int k = 0; k < iterations; k++) {
        sum1 = 0;
        sum2 = 0;
        for (int i = 0; i < N; i++) {
            sum1 += (*(y.data + i) - b0 - b1 * *(x.data + i));
            sum2 += (*(y.data + i) - b0 - b1 * *(x.data + i)) * *(x.data + i);
        }
        b0 = b0 - rate * (-2 * sum1 / N);
        b1 = b1 - rate * (-2 * sum2 / N);
    }

    results[0] = b0;
    results[1] = b1;
    return results;
}

template <typename T>
Vector<double> generalFit(const T* x, const T* y, const unsigned int& amount, const unsigned int& degree = 1, const unsigned int& iterations = 15, const double& rate = 0.0000002) {
    if (degree == 0 or iterations == 0 or amount == 0) throw RangeError();

    auto b = Vector<double>::one(degree + 1);

    // Function globals
    Vector<double> c;
    Vector<double> v;
    Vector<double> factor;
    double temp[amount];
    double coefficient = -2 * rate / amount;

    for (int k = 0; k < iterations; k++) {
        c = Vector<double>::zero(amount);
        for (int i = 0; i < amount; i++) {
            for (int j = 0; j < degree; j++) {
                *(temp + i) = pow(*(x + i), j);
            }
            v = Vector<double>(degree, temp);
            factor = (*(y + i) - (b * v)) * c;
            c += factor;

            factor.clear();
            v.clear();
        }
        c *= coefficient;
        factor = b - c; // This will generate another vector
        b.clear();
        c.clear();
        b = factor;
    }
    return b;
}

template <typename T>
Vector<double> generalFit(const Vector<T>& x, const Vector<T>& y, const unsigned int& degree = 1, const unsigned int& iterations = 15, const double& rate = 0.0000002) {
    const unsigned int N = x.length;
    if (degree == 0 or iterations == 0 or N == 0 or y.length == 0) throw RangeError();
    if (N != y.length) throw DimensionError();

    auto b = Vector<double>::one(degree + 1);
    // Function globals
    Vector<double> c;
    Vector<double> v;
    Vector<double> factor;
    double temp[N];
    double coefficient = -2 * rate / N;

    for (int k = 0; k < iterations; k++) {
        c = Vector<double>::zero(N);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < degree; j++) {
                *(temp + i) = pow(*(x.data + i), j);
            }
            v = Vector<double>(degree, temp);
            factor = (*(y.data + i) - (b * v)) * c;
            v.clear();
            c += factor;
            factor.clear();
        }
        c *= coefficient;
        factor = b - c; // This will generate another vector
        b.clear();
        c.clear();
        b = factor;
    }
    return b;
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
complex<double> frobeniusProduct(const Matrix<complex<T>>& m, const Matrix<complex<U>>& n) {
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
complex<double> frobeniusProduct(const Matrix<T>& m, const Matrix<complex<U>>& n) {
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
double frobeniusProduct(const Matrix<T>& m, const Matrix<U>& n) {
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

/*
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

 */

#endif //VECTOR_CPP_VECTORGEBRA_H