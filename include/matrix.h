//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_MATRIX_H
#define VECTOR_CPP_MATRIX_H

#include "vector.h"

template <typename T>
class Matrix {

    template <typename U>
    using ctype = typename std::common_type<T, U>::type;

public:

    Vector<Vector<T>*> *data = nullptr;
    unsigned int a = 0;
    unsigned int b = 0;

    Matrix() = default;

    Matrix<T>(const unsigned int& a, const unsigned int& b, Vector<T>* v_list) {
        auto* new_data = new Vector<Vector<T>*>[a];
        for (int i = 0; i < a; i++) {
            auto* to_add = new Vector<T>(b, (v_list + i)->data);
            new_data->append(to_add);
        }
        this->data = new_data;
        this->a = a;
        this->b = b;
    }

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
    }

    void append(const Vector<T>& v) {
        if (v.length != this->b and this->b != 0) throw DimensionError();
        if (this->b == 0) {
            this->b = v.length;
        }
        if (this->data == nullptr) {
            this->data = new Vector<Vector<T>*>;
        }
        auto to_add = new Vector<T>(v.length, v.data);
        this->data->append(to_add);
        this->a += 1;
    }

    Vector<T> pop(int index = -1) {
        Vector<T>* popped = this->data->pop(index);
        Vector<T> result(this->b, popped->data);
        popped->clear();
        delete popped;
        this->a -= 1;
        return result;
    }

    void insert(int index, const Vector<T>& v) {
        if (v.length != this->b and this->b != 0) throw DimensionError();
        if (this->b == 0) {
            this->b = v.length;
        }
        auto to_add = new Vector<T>(v.length, v.data);
        if (this->data == nullptr) {
            this->data = new Vector<Vector<T>*>[2];
            this->data->data = to_add;
            this->a = 1;
            return;
        }
        if (index < 0) index += this->a;
        if (index < 0) {
            to_add.clear();
            delete to_add;
            throw IndexError();
        }
        // Without error checking here, if the error occurs in the Vector->insert,
        // Then we have a memory leak
        this->data->insert(index, to_add);
    }

    void clear() {
        if (this->a == 0) return;
        for (int i = 0; i < this->a; i++) {
            (*(this->data->data + i))->clear();
        }
        this->data->clear();
    }

    Matrix<T> copy() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = *(v->data + j);
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    void sort(const bool& reverse = false) {
        if (this->a == 0) return;
        Vector<T>* temp;
        // Sorts by pivot indexes
        if (reverse) {
            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((*(this->data->data + j))->__pivot() < (*(this->data->data + j + 1))->__pivot()) {
                        temp = *(this->data->data + j);
                        *(this->data->data + j) = *(this->data->data + j + 1);
                        *(this->data->data + j + 1) = temp;
                    }
                }
            }
        } else {
            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((*(this->data->data + j))->__pivot() > (*(this->data->data + j + 1))->__pivot()) {
                        temp = *(this->data->data + j);
                        *(this->data->data + j) = *(this->data->data + j + 1);
                        *(this->data->data + j + 1) = temp;
                    }
                }
            }
        }
    }

    void resize() {
        this->data->resize();
    }

    void reverse() {
        this->data->reverse();
    }

    unsigned int getsize() {
        return this->data->getsize();
    }

    Vector<T> operator[] (int index) const {
        if (this->a == 0) throw IndexError();
        if (index < 0) index += this->a;
        if (index < 0 or index >= this->a) throw IndexError();
        return **(this->data->data + index);
    }

    [[nodiscard]] Matrix<T> transpose() const {
        Vector<T> new_data[this->b];
        Vector<T>** v;
        for (int i = 0; i < this->b; i++) {
            T temp[this->a];
            v = (this->data->data);
            for (int j = 0; j < this->a; j++) {
                temp[j] = *((*(v + j))->data + i);
            }
            Vector<T> v_temp(this->a, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->b, this->a, new_data);
        // Below loop clears the temporary vectors
        for (int i = 0; i < this->b; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    Matrix<T> operator+ (const T& val) const {
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) + val;
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

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
    }

    template <typename U>
    Matrix<ctype<U>> operator+ (const U& val) const {
        Vector<ctype<U>> new_data[this->a];
        auto c = static_cast<ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) + c;
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

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
    }

    Matrix<T> operator+ (const Matrix<T>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<T> new_data[this->a];
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) + *(w->data + j);
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator+ (const Matrix<U>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<ctype<U>> new_data[this->a];
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) + static_cast<ctype<U>>(*(w->data + j));
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    Matrix<T> operator+= (const T& val) {
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += val;
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator+= (const U& val) {
        T temp = static_cast<T>(val);
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += temp;
            }
        }
        return *this;
    }

    Matrix<T> operator+= (const Matrix<T>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += *(w->data + j);
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator+= (const Matrix<U>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += static_cast<T>(*(w->data + j));
            }
        }
        return *this;
    }

    Matrix<T> operator- () const {
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = - *(v->data + j);
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    Matrix<T> operator- (const T& val) const {
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) - val;
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

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
    }

    template <typename U>
    Matrix<ctype<U>> operator- (const U& val) const {
        Vector<ctype<U>> new_data[this->a];
        auto c = static_cast<ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) - c;
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    friend Matrix<ctype<U>> operator- (const U& val, const Matrix<T>& m) {
        Vector<ctype<U>> new_data[m.a];
        auto c = static_cast<ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < m.a; i++) {
            ctype<U> temp[m.b];
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
    }

    Matrix<T> operator- (const Matrix<T>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<T> new_data[this->a];
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) - *(w->data + j);
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator- (const Matrix<U>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<ctype<U>> new_data[this->a];
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) - static_cast<ctype<U>>(*(w->data + j));
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    Matrix<T> operator-= (const T& val) {
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= val;
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator-= (const U& val) {
        T temp = static_cast<T>(val);
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= temp;
            }
        }
        return *this;
    }

    Matrix<T> operator-= (const Matrix<T>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= *(w->data + j);
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator-= (const Matrix<U>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= static_cast<T>(*(w->data + j));
            }
        }
        return *this;
    }

    Matrix<T> operator* (const T& val) const {
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) * val;
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

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
    }

    template <typename U>
    Matrix<ctype<U>> operator* (const U& val) const {
        Vector<ctype<U>> new_data[this->a];
        auto c = static_cast<ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) * c;
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    friend Matrix<ctype<U>> operator* (const U& val, const Matrix<T>& m) {
        Vector<ctype<U>> new_data[m.a];
        auto c = static_cast<ctype<U>>(val);
        Vector<T>* v;
        for (int i = 0; i < m.a; i++) {
            ctype<U> temp[m.b];
            v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                *(temp + j) = c * static_cast<ctype<U>>(*(v->data + j));
            }
            Vector<ctype<U>> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(m.a, m.b, new_data);
        for (int i = 0; i < m.a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    Matrix<T> operator* (const Matrix<T>& m) const {
        if (this->b != m.a) throw DimensionError();
        Vector<T> new_data[this->a]; // m.b is the length of each vector
        Vector<T>* v;

        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i); // pointer to basis vector copied, ~%2.5 faster for high dimensional matrices.
            T temp[m.b];
            for (int j = 0; j < m.b; j++) {
                T sum = 0;
                for (int k = 0; k < m.a; k++) {
                    sum += *(v->data + k) * *((*(m.data->data + k))->data + j);
                }
                *(temp + j) = sum;
            }
            Vector<T> to_add(m.b, temp);
            *(new_data + i) = to_add;
        }
        Matrix<T> result(this->a, m.b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator* (const Matrix<U>& m) const {
        if (this->b != m.a) throw DimensionError();
        Vector<ctype<U>> new_data[this->a]; // m.b is the length of each vector
        Vector<T>* v;

        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i); // basis vector copied
            ctype<U> temp[m.b];
            for (int j = 0; j < m.b; j++) {
                ctype<U> sum = 0;
                for (int k = 0; k < m.a; k++) {
                    sum += static_cast<ctype<U>>(*(v->data + k)) * static_cast<ctype<U>>(*((*(m.data->data + k))->data + j));
                }
                *(temp + j) = sum;
            }
            Vector<ctype<U>> to_add(m.b, temp);
            *(new_data + i) = to_add;
        }
        Matrix<ctype<U>> result(this->a, m.b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    Vector<T> operator* (const Vector<T>& v) const {
        if (this->b != v.length) throw DimensionError();
        T temp[this->a];
        auto v_data = v.data;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            w = *(this->data->data + i);
            T sum = 0;
            for (int j = 0; j < this->b; j++) {
                sum += *(w->data + j) * *(v_data + j);
            }
            *(temp + i) = sum;
        }
        Vector<T> result(this->b, temp); // Heap only allocated here, and pointer to it is returned below
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator* (const Vector<U>& v) const {
        if (this->b != v.length) throw DimensionError();
        ctype<U> temp[this->a];
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            w = *(this->data->data + i);
            ctype<U> sum = 0;
            for (int j = 0; j < this->b; j++) {
                sum += *(w->data + j) * *(v.data + j);
            }
            *(temp + i) = sum;
        }
        Vector<ctype<U>> result(this->b, temp);
        return result;
    }

    Matrix<T> operator*= (const T& val) {
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) *= val;
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator*= (const U& val) {
        auto c = static_cast<T>(val);
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) *= c;
            }
        }
        return *this;
    }

    Matrix<T> operator*= (const Matrix<T>& m) {
        if (this->b != m.a) throw DimensionError();
        Vector<T> new_data[this->a]; // m.b is the length of each vector
        Vector<T>* v;

        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i); // pointer to basis vector copied, ~%2.5 faster for high dimensional matrices.
            T temp[m.b];
            for (int j = 0; j < m.b; j++) {
                T sum = 0;
                for (int k = 0; k < m.a; k++) {
                    sum += *(v->data + k) * *((*(m.data->data + k))->data + j);
                }
                *(temp + j) = sum;
            }
            Vector<T> to_add(m.b, temp);
            *(new_data + i) = to_add;
        }

        Matrix<T> result(this->a, m.b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        this->clear();
        *this = result;
        return *this;
    }

    template <typename U>
    Matrix<double> operator/ (const U& val) const {
        Vector<T>* v;
        Vector<double> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            double temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<double>(*(v->data + j)) / val;
            }
            Vector<double> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<double> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    bool operator== (const Matrix<T>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(v->data + j) != *(w->data + j)) return false;
            }
        }
        return true;
    }

    template <typename U>
    bool operator== (const Matrix<U>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(v->data + j) != *(w->data + j)) return false;
            }
        }
        return true;
    }

    bool operator!= (const Matrix<T>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(v->data + j) == *(w->data + j)) return false;
            }
        }
        return true;
    }

    template <typename U>
    bool operator!= (const Matrix<U>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(v->data + j) == *(w->data + j)) return false;
            }
        }
        return true;
    }

    template <typename U>
    Matrix<bool> operator|| (const Matrix<U>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<bool> new_data[this->a];
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            bool temp[this->b];
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = *(v->data + j) || *(w->data + j);
            }
            *(new_data + i) = Vector<bool>(this->b, temp);
        }
        Matrix<bool> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    Matrix<bool> operator&& (const Matrix<U>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<bool> new_data[this->a];
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            bool temp[this->b];
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = *(v->data + j) && *(w->data + j);
            }
            *(new_data + i) = Vector<bool>(this->b, temp);
        }
        Matrix<bool> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    Matrix<bool> operator^ (const Matrix<U>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        Vector<bool> new_data[this->a];
        Vector<T>* v;
        Vector<T>* w;
        for (int i = 0; i < this->a; i++) {
            bool temp[this->b];
            v = *(this->data->data + i);
            w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = *(v->data + j) ^ *(w->data + j);
            }
            *(new_data + i) = Vector<bool>(this->b, temp);
        }
        Matrix<bool> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] Matrix<int> toInt() const {
        Vector<int> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            int temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<int>(*(v->data + j));
            }
            Vector<int> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<int> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] Matrix<float> toFloat() const {
        Vector<float> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            float temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<float>(*(v->data + j));
            }
            Vector<float> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<float> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] Matrix<double> toDouble() const {
        Vector<double> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            double temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<double>(*(v->data + j));
            }
            Vector<double> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<double> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] Matrix<bool> toBool() const {
        Vector<bool> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            bool temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<bool>(*(v->data + j));
            }
            Vector<bool> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<bool> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    static Matrix<T> zero(const unsigned int& a, const unsigned int& b) {
        Vector<T> new_data[a];
        for (int i = 0; i < a; i++) {
            T temp[b];
            for (int j = 0; j < b; j++) {
                *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(b, temp);
        }
        Matrix<T> result(a, b, new_data);
        for (int i = 0; i < a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    static Matrix<T> one(const unsigned int& a, const unsigned int& b) {
        Vector<T> new_data[a];
        for (int i = 0; i < a; i++) {
            T temp[b];
            for (int j = 0; j < b; j++) {
                *(temp + j) = 1;
            }
            *(new_data + i) = Vector<T>(b, temp);
        }
        Matrix<T> result(a, b, new_data);
        for (int i = 0; i < a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    static Matrix<T> identity(const unsigned int& d) {
        Vector<T> new_data[d];
        for (int i = 0; i < d; i++) {
            T temp[d];
            for (int j = 0; j < d; j++) {
                if (i == j) *(temp + j) = 1;
                else *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(d, temp);
        }
        Matrix<T> result(d, d, new_data);
        for (int i = 0; i < d; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    static Matrix<int> randMint(const unsigned int& m, const unsigned int& n, const int& a, const int& b) {
        Vector<int> new_data[m];
        for (int i = 0; i < m; i++) {
            int temp[n];
            for (int j = 0; j < n; j++) {
                *(temp + j) = randint(a, b);
            }
            *(new_data + i) = Vector<int>(n, temp);
        }
        Matrix<int> result(m, n, new_data);
        for (int i = 0; i < m; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    static Matrix<float> randMfloat(const unsigned int& m, const unsigned int& n, const float& a, const float& b) {
        Vector<float> new_data[m];
        for (int i = 0; i < m; i++) {
            float temp[n];
            for (int j = 0; j < n; j++) {
                *(temp + j) = randfloat(a, b);
            }
            *(new_data + i) = Vector<float>(n, temp);
        }
        Matrix<float> result(m, n, new_data);
        for (int i = 0; i < m; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    static Matrix<double> randMdouble(const unsigned int& m, const unsigned int& n, const double& a, const double& b) {
        Vector<double> new_data[m];
        for (int i = 0; i < m; i++) {
            double temp[n];
            for (int j = 0; j < n; j++) {
                *(temp + j) = randdouble(a, b);
            }
            *(new_data + i) = Vector<double>(n, temp);
        }
        Matrix<double> result(m, n, new_data);
        for (int i = 0; i < m; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    static Matrix<bool> randMbool(const unsigned int&m, const unsigned int& n) {
        Vector<bool> new_data[m];
        for (int i = 0; i < m; i++) {
            bool temp[n];
            for (int j = 0; j < n; j++) {
                *(temp + j) = randbool();
            }
            *(new_data + i) = Vector<bool>(n, temp);
        }
        Matrix<bool> result(m, n, new_data);
        for (int i = 0; i < m; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] Matrix<T> cumsum() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }

        Vector<T> list[this->a];
        T remainder = 0;
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            T temp[this->b];
            *temp = *(v->data) + remainder;
            for (int j = 1; j < this->b; j++) {
                *(temp + j) = *(temp + j - 1) + *(v->data + j);
            }
            Vector<T> temp_v(this->b, temp);
            *(list + i) = temp_v;
            remainder = *(temp + this->b - 1);
        }
        Matrix<T> result(this->a, this->b, list);
        for (int i = 0; i < this->a; i++) {
            (list + i)->clear();
        }
        return result;
    }

    [[nodiscard]] T sum() const {
        if (this->a == 0) return 0;
        T sum = 0;
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                sum += *(v->data + j);
            }
        }
        return sum;
    }

    [[nodiscard]] Matrix<T> getDiagonal() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }
        if (this->a != this->b) throw DimensionError();
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (i == j) *(temp + j) = *(v->data + j);
                else *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] Matrix<T> getUpper() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }
        if (this->a != this->b) throw DimensionError();
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (i < j) *(temp + j) = *(v->data + j);
                else *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] Matrix<T> getLower() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }
        if (this->a != this->b) throw DimensionError();
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (i > j) *(temp + j) = *(v->data + j);
                else *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    template <typename U>
    Matrix<U> map(std::function<U(T)> f) const {
        if (this->a == 0) {
            Matrix<U> result;
            return result;
        }
        if (this->a != this->b) throw DimensionError();
        Vector<T> new_data[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = f(*(v->data + j));
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        for (int i = 0; i < this->a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }

    [[nodiscard]] T mulDiagonal() const {
        if (this->a == 0) return 0;
        if (this->a != this->b) throw DimensionError();
        T mul = 1;
        for (int i = 0; i < this->a; i++) {
            mul *= *((*(this->data->data + i))->data + i);
        }
        return mul;
    }

    [[nodiscard]] T trace() const {
        if (this->a == 0) return 0;
        if (this->a != this->b) throw DimensionError();
        T sum = 0;
        for (int i = 0; i < this->a; i++) {
            sum += *((*(this->data->data + i))->data + i);
        }
        return sum;
    }

    [[nodiscard]] int* __pivots() const {
        if (this->a == 0) return nullptr;
        auto plist = new int[this->a];
        Vector<T>* v;
        for (int i = 0; i < this->a; i++) {
            v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(v->data + j) != 0) {
                    *(plist + i) = j;
                    break;
                } else if (j == this->b - 1) {
                    *(plist + i) = this->b;
                }
            }
        }
        return plist;
    }

    Matrix<double> echelon(const double& lowlimit = 0.0000000001, const double& highlimit = 1000000) {
        Vector<double>* temp;
        Vector<double>* v;
        Vector<double>* w;
        double first, rows_first;
        int first_index;
        double factor;
        auto copy_matrix = this->toDouble(); // We will return this, we cannot clear it.

        // Bubble sort
        for (int i = 0; i < this->a; i++) {
            for (int j = 0; j < this->a - i - 1; j++) {
                if ((**(copy_matrix.data->data + j)).__pivot() > (**(copy_matrix.data->data + j + 1)).__pivot()) {
                    temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                    *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                    *(copy_matrix.data->data + j + 1) = temp;
                }
            }
        }

        for (int i = 0; i < this->a; i++) {
            v = *(copy_matrix.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (abs(*(v->data + j)) > lowlimit) {
                    first = *(v->data + j);
                    first_index = j;
                    break;
                }
            }
            for (int r = i + 1; r < this->a; r++) {
                w = *(copy_matrix.data->data + r); // pointer to vector
                rows_first = *(w->data + first_index);
                if (abs(rows_first) < lowlimit) continue;
                factor = rows_first / first;
                if (abs(factor) > highlimit) continue;
                *w -= factor * *v;
            }
        }

        for (int i = 0; i < this->a; i++) {
            for (int j = 0; j < this->a - i - 1; j++) {
                if ((**(copy_matrix.data->data + j)).__pivot() > (**(copy_matrix.data->data + j + 1)).__pivot()) {
                    temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                    *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                    *(copy_matrix.data->data + j + 1) = temp;
                }
            }
        }
        return copy_matrix;
    }

    Matrix<double> rrechelon(const double& lowlimit = 0.0000000000000000000001, const double& highlimit = 1000000000) {
        Vector<double>* temp;
        Vector<double>* v;
        Vector<double>* w;
        double first, rows_first;
        int first_index;
        double factor;
        auto copy_matrix = this->toDouble(); // We will return this, we cannot clear it.

        // Bubble sort
        for (int i = 0; i < this->a; i++) {
            for (int j = 0; j < this->a - i - 1; j++) {
                if ((**(copy_matrix.data->data + j)).__pivot() > (**(copy_matrix.data->data + j + 1)).__pivot()) {
                    temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                    *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                    *(copy_matrix.data->data + j + 1) = temp;
                }
            }
        }

        // Core of the reduction algorithm
        // copy_matrix gets reduced.
        for (int i = 0; i < this->a; i++) {
            v = *(copy_matrix.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (abs(*(v->data + j)) > lowlimit) { // Assume zero if below lowlimit
                    first = *(v->data + j);
                    first_index = j;
                    break;
                }
            }
            for (int r = 0; r < this->a; r++) {
                if (i == r) continue;
                w = *(copy_matrix.data->data + r);
                rows_first = *(w->data + first_index);
                if (abs(rows_first) < lowlimit) continue;
                factor = rows_first / first;
                if (abs(factor) > highlimit or abs(factor) < lowlimit) continue;
                *w -= factor * *v;
            }
        }

        // Reverse the order of rows and reduce it again
        // to get a Reduced Row Echelon form.
        copy_matrix.reverse(); // Does not cause any memory leaks, works on self.

        // Reduce again.
        for (int i = 0; i < this->a; i++) {
            v = *(copy_matrix.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (abs(*(v->data + j)) > lowlimit) {
                    first = *(v->data + j);
                    first_index = j;
                    break;
                }
            }
            for (int r = 0; r < this->a; r++) {
                if (i == r) continue;
                w = *(copy_matrix.data->data + r);
                rows_first = *(w->data + first_index);
                if (abs(rows_first) < lowlimit) continue;
                factor = rows_first / first;
                if (abs(factor) > highlimit or abs(factor) < lowlimit) continue;
                *w -= factor * *v;
            }
        }

        for (int i = 0; i < this->a; i++) {
            for (int j = 0; j < this->a - i - 1; j++) {
                if ((**(copy_matrix.data->data + j)).__pivot() > (**(copy_matrix.data->data + j + 1)).__pivot()) {
                    temp = *(copy_matrix.data->data + j);
                    *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                    *(copy_matrix.data->data + j + 1) = temp;
                }
            }
        }

        // Error check on the last row
        // It is guaranteed that below operations won't change the
        // supposed row order
        v = *(copy_matrix.data->data + this->a - 1);
        first = *(v->data + this->b - 1);
        if (abs(first) > lowlimit and abs(first) < highlimit) {
            for (int i = 0; i < this->a - 1; i++) {
                w = *(copy_matrix.data->data + i);
                rows_first = *(w->data + this->b - 1);
                if (abs(rows_first) < lowlimit) continue;
                factor = rows_first / first;
                if (abs(factor) > highlimit or abs(factor) < lowlimit) continue;
                *w -= factor * *v;
            }
        }

        return copy_matrix;
    }

    double determinant(const std::string& method = "echelon", const double& lowlimit = 0.0000000001, const double& highlimit = 1000000) {
        if (this->a != this->b) throw DimensionError();
        if (this->a == 1) {
            return *((*(this->data->data))->data);
        }
        if (method == "analytic") {
            Vector<T> v_list[this->a];
            for (int i = 0; i < this->a; i++) {
                *(v_list + i) = **(this->data->data + i);
            }
            return Vector<T>::determinant(this->a, v_list);
        } else if (method == "echelon") {
            // Let's prevent the extra function call
            Vector<double>* temp;
            Vector<double>* v;
            Vector<double>* w;
            double first, rows_first;
            int first_index;
            double factor;
            bool sign = true;
            auto copy_matrix = this->toDouble(); // We will return this, we cannot clear it.

            // Bubble sort
            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((**(copy_matrix.data->data + j)).__pivot() > (**(copy_matrix.data->data + j + 1)).__pivot()) {
                        temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                        *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                        *(copy_matrix.data->data + j + 1) = temp;
                        sign = !sign;
                    }
                }
            }

            for (int i = 0; i < this->a; i++) {
                v = *(copy_matrix.data->data + i);
                for (int j = 0; j < this->b; j++) {
                    if (abs(*(v->data + j)) > lowlimit) {
                        first = *(v->data + j);
                        first_index = j;
                        break;
                    }
                }
                for (int r = i + 1; r < this->a; r++) {
                    w = *(copy_matrix.data->data + r); // pointer to vector
                    rows_first = *(w->data + first_index);
                    if (abs(rows_first) < lowlimit) continue;
                    factor = rows_first / first;
                    if (abs(factor) > highlimit) continue;
                    *w -= factor * *v;
                }
            }

            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((**(copy_matrix.data->data + j)).__pivot() > (**(copy_matrix.data->data + j + 1)).__pivot()) {
                        temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                        *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                        *(copy_matrix.data->data + j + 1) = temp;
                        sign = !sign;
                    }
                }
            }

            auto result = copy_matrix.mulDiagonal();
            copy_matrix.clear();
            if (sign) return result;
            return -result;
        }
    }

    [[nodiscard]] Matrix<double> inverse(const std::string& method = "iterative", const unsigned int& resolution = 15,
                                         const double& lowlimit = 0.0000000001, const double& highlimit = 1000000) const {
        if (this->a == 0 or (this->a != this->b)) throw DimensionError();

        if (method == "iterative") {
            auto the_double = this->toDouble();
            auto tpose = the_double.transpose();
            auto control_matrix = the_double * tpose;

            double sum_list[control_matrix.a];
            Vector<double>* v;
            for (int i = 0; i < control_matrix.a; i++) {
                v = *(control_matrix.data->data + i);
                double sum = 0;
                for (int j = 0; j < control_matrix.b; j++) {
                    sum += abs(*(v->data + j));
                }
                *(sum_list + i) = sum;
            }

            double maxima = *sum_list;
            for (int i = 1; i < control_matrix.a; i++) {
                if (*(sum_list + i) > maxima) {
                    maxima = *(sum_list + i);
                }
            }

            auto guess = tpose / maxima;
            auto iden = Matrix<double>::identity(this->a);
            iden *= 2;

            for (int i = 0; i < resolution; i++) {
                guess = guess * (iden - the_double * guess);
            }

            // What if i make clear method act like "async"?
            // At this point, we can just return the "guess" and
            // Apply deletions in the background. No big deal.
            control_matrix.clear();
            iden.clear();
            the_double.clear();
            tpose.clear();
            return guess;
        }
        else if (method == "gauss") {
            Vector<double>* temp;
            Vector<double>* v;
            Vector<double>* w;

            // Mirror pointers
            Vector<double>* v_i;
            Vector<double>* w_i;

            double first, rows_first, factor;
            int first_index;
            auto copy_matrix = this->toDouble(); // We will return this, we cannot clear it.
            auto iden = Matrix<double>::identity(this->a);

            // Bubble sort
            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((*(copy_matrix.data->data + j))->__pivot() > (*(copy_matrix.data->data + j + 1))->__pivot()) {
                        // Swap the real matrix
                        temp = *(copy_matrix.data->data + j);
                        *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                        *(copy_matrix.data->data + j + 1) = temp;

                        // Swap the mirror matrix
                        temp = *(iden.data->data + j);
                        *(iden.data->data + j) = *(iden.data->data + j + 1);
                        *(iden.data->data + j + 1) = temp;
                    }
                }
            }

            // Core of the reduction algorithm
            for (int i = 0; i < this->a; i++) {
                v = *(copy_matrix.data->data + i);
                v_i = *(iden.data->data + i);
                for (int j = 0; j < this->a; j++) {
                    if (abs(*(v->data + j)) > lowlimit) {
                        first = *(v->data + j);
                        first_index = j;
                        break;
                    }
                }

                for (int r = 0; r < this->a; r++) {
                    if (i == r) continue;
                    w = *(copy_matrix.data->data + r);
                    w_i = *(iden.data->data + r);

                    rows_first = *(w->data + first_index);
                    if (abs(rows_first) < lowlimit) continue;
                    factor = rows_first / first;
                    if (abs(factor) > highlimit or abs(factor) < lowlimit) continue;
                    *w -= factor * *v;
                    *w_i -= factor * *v_i; // Mirror operation
                }
            }

            // There is a possibility that below code can be written
            // without applying .reverse() operation.
            // Just changing the indexes of the loops is enough
            copy_matrix.reverse();
            iden.reverse();

            // Reduce again.
            for (int i = 0; i < this->a; i++) {
                v = *(copy_matrix.data->data + i);
                v_i = *(iden.data->data + i);
                for (int j = 0; j < this->b; j++) {
                    if (abs(*(v->data + j)) > lowlimit) {
                        first = *(v->data + j);
                        first_index = j;
                        break;
                    }
                }
                for (int r = i + 1; r < this->a; r++) {
                    w = *(copy_matrix.data->data + r);
                    w_i = *(iden.data->data + r);

                    rows_first = *(w->data + first_index);
                    if (abs(rows_first) < lowlimit) continue;
                    factor = rows_first / first;
                    if (abs(factor) > highlimit or abs(factor) < lowlimit) continue;
                    *w -= factor * *v;
                    *w_i -= factor * *v_i;
                }
            }

            // Sort again.
            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((*(copy_matrix.data->data + j))->__pivot() > (*(copy_matrix.data->data + j + 1))->__pivot()) {
                        temp = *(copy_matrix.data->data + j);
                        *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                        *(copy_matrix.data->data + j + 1) = temp;

                        temp = *(iden.data->data + j);
                        *(iden.data->data + j) = *(iden.data->data + j + 1);
                        *(iden.data->data + j + 1) = temp;
                    }
                }
            }

            v = *(copy_matrix.data->data + this->a - 1);
            v_i = *(iden.data->data + this->a - 1);
            first = *(v->data + this->b - 1);
            if (abs(first) > lowlimit and abs(first) < highlimit) {
                for (int i = 0; i < this->a - 1; i++) {
                    w = *(copy_matrix.data->data + i);
                    w_i = *(iden.data->data + i);
                    rows_first = *(w->data + this->b - 1);
                    if (abs(rows_first) < lowlimit) continue;
                    factor = rows_first / first;
                    if (abs(factor) > highlimit or abs(factor) < lowlimit) continue;
                    *w -= factor * *v;
                    *w_i -= factor * *v_i;
                }
            }

            for (int i = 0; i < this->a; i++) {
                first = *((*(copy_matrix.data->data + i))->data + i);
                if (abs(first) < lowlimit or abs(first) > highlimit) continue;
                v_i = *(iden.data->data + i);
                for (int j = 0; j < this->a; j++) {
                    *(v_i->data + j) /= first;
                    auto val = *(v_i->data + j);
                    if (abs(val) < lowlimit or abs(val) > highlimit) {
                        *(v_i->data + j) = 0;
                    }
                }
            }

            copy_matrix.clear();
            return iden;
        }

        throw ArgumentError();
    }

    static Matrix<double> givens(const unsigned int& dim, const unsigned int& i, const unsigned int& j,
                                 const double& angle, const unsigned int& resolution = 15) {
        if (i >= dim or j >= dim or resolution == 0) throw RangeError();

        auto result = Matrix<double>::identity(dim);
        auto c = cos(angle, resolution);
        auto s = sin(angle, resolution);

        *((*(result.data->data + i))->data + i) = c;
        *((*(result.data->data + j))->data + j) = c;
        *((*(result.data->data + i))->data + j) = s;
        *((*(result.data->data + j))->data + i) = -s;
        return result;
    }

    template <typename U>
    Vector<ctype<U>> leastSquares(const Vector<U>& v, const std::string& method = "iterative", const unsigned int& resolution = 15,
                                const double& lowlimit = 0.0000000001, const double& highlimit = 1000000) const {
        if (this->a != v.length or this->a == 0) throw DimensionError();

        auto tpose = this->transpose();
        auto temp = (tpose * *this).inverse(method, resolution, lowlimit, highlimit);
        auto result = temp * (tpose * v);
        temp.clear();
        tpose.clear();
        return result;
    }

    Matrix<double>* QR(const double& limiter = 0.0000000001) const {
        if (this->a == 0) return nullptr;

        Vector<double> vlist[this->b];
        auto the_double = this->toDouble();
        auto tpose = the_double.transpose();

        for (int i = 0; i < this->b; i++) {
            *(vlist + i) = **(tpose.data->data + i); // Direct copy of the columns of the self
        }

        // Definition of spanify
        *vlist = vlist->unit();
        for (int i = 1; i < this->b; i++) {
            for (int j = 0; j < i; j++) {
                *(vlist + i) -= (vlist + i)->proj(*(vlist + j));
            }
            *(vlist + i) = (vlist + i)->unit();
        }

        for (int i = 0; i < this->b; i++) {
            for (int j = i + 1; j < this->b; j++) {
                if (*(vlist + i) * *(vlist + j) > limiter) {
                    // Memory cleaning before returning
                    the_double.clear();
                    tpose.clear();
                    return nullptr; // The false case of vector.doesSpan
                }
            }
        }

        auto results = new Matrix<double>[2];

        auto Q_t = Matrix<double>(this->b, this->a, vlist); // Will be cleared later

        Vector<double> new_data[Q_t.b];
        for (int i = 0; i < Q_t.b; i++) {
            double temp[Q_t.a];
            auto v = (Q_t.data->data);
            for (int j = 0; j < Q_t.a; j++) {
                temp[j] = *((*(v + j))->data + i);
            }
            Vector<double> v_temp(Q_t.a, temp);
            *(new_data + i) = v_temp;
        }
        auto Q = Matrix<double>(Q_t.b, Q_t.a, new_data);
        // Below loop clears the temporary vectors
        for (int i = 0; i < Q_t.b; i++) {
            (new_data + i)->clear();
        }

        // Q_t * *this
        Vector<double> new_data2[Q_t.a];
        for (int i = 0; i < Q_t.a; i++) {
            auto v = *(Q_t.data->data + i);
            double temp[this->b];
            for (int j = 0; j < this->b; j++) {
                double sum = 0;
                for (int k = 0; k < this->a; k++) {
                    sum += *(v->data + k) * *((*(this->data->data + k))->data + j);
                }
                *(temp + j) = sum;
            }
            Vector<double> to_add(this->b, temp);
            *(new_data2 + i) = to_add;
        }

        auto R = Matrix<double>(Q_t.a, this->b, new_data2);
        for (int i = 0; i < Q_t.a; i++) {
            (new_data2 + i)->clear();
        }

        Q_t.clear();
        the_double.clear();
        tpose.clear();
        results[0] = Q;
        results[1] = R;
        return results;
    }

    [[nodiscard]] double* eigenvalue(const unsigned int& resolution = 15) const {
        // This will return a heap allocated array of length this->a
        if (this->a != this->b) throw DimensionError();
        if (resolution == 0) throw RangeError();

        auto the_double = this->toDouble();
        for (int i = 0; i < resolution; i++) {
            auto qr = the_double.QR(); // Allocates heap
            the_double.clear(); // ALWAYS clear before REASSIGNMENT

            the_double = qr[1] * qr[0];

            qr[1].clear();
            qr[0].clear();
            delete[] qr;
        }

        auto results = new double[this->a];
        // Data transfer
        for (int i = 0; i < this->a; i++) {
            *(results + i) = *((*(the_double.data->data + i))->data + i);
        }
        the_double.clear(); // It exited the loop filled
        return results;
    }

    template <typename U>
    static double frobeniousProduct(const Matrix<T>& m, const Matrix<U>& n) {
        // T or U CANNOT be complex
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

    // Negative indexing is allowed here
    [[nodiscard]] Matrix<T> submatrix(int a, int b, int c, int d) const {
        if (a < 0) a += this->a; // from [
        if (b < 0) b += this->a; // until )
        if (c < 0) c += this->b; // from [
        if (d < 0) d += this->b; // until )
        if ((a < 0 or a > this->a) or (b < 0 or b > this->a) or a >= b
        or (c < 0 or c > this->b) or (d < 0 or d > this->b) or c >= d) throw RangeError();

        Vector<T> new_data[b - a];
        for (int i = a; i < b; i++) {
            auto v = *(this->data->data + i);
            T temp[d - c];
            for (int j = c; j < d; j++) {
                *(temp + j - c) = *(v->data + j);
            }
            *(new_data + i - a) = Vector<T>(d - c, temp);
        }
        Matrix<T> result(b - a, d - c, new_data);
        for (int i = 0; i < b - a; i++) {
            (new_data + i)->clear();
        }
        return result;
    }
};

#endif //VECTOR_CPP_MATRIX_H
