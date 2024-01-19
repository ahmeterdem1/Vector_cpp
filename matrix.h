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
        for (i = 0; i < m.a; i++) {
            Vector<T>* v = *(m.data->data + i);
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
        for (int i = 0; i < this->b; i++) {
            T temp[this->a];
            auto v = *(this->data->data);
            for (int j = 0; j < this->a; j++) {
                temp[j] = *((v->data + j) + i);
            }
            Vector<T> v_temp(this->a, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->b, this->a, new_data);
        return result;
    }

    Matrix<T> operator+ (const T& val) const {
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) + val;
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
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
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator+ (const U& val) const {
        Vector<ctype<U>> new_data[this->a];
        auto c = static_cast<ctype<U>>(val);
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) + c;
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        return result;
    }

    template <typename U>
    friend Matrix<ctype<U>> operator+ (const U& val, const Matrix<T>& m) {
        Vector<ctype<U>> new_data[m.a];
        auto c = static_cast<ctype<U>>(val);
        for (int i = 0; i < m.a; i++) {
            ctype<U> temp[m.b];
            auto v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) + c;
            }
            Vector<ctype<U>> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(m.a, m.b, new_data);
        return result;
    }

    Matrix<T> operator+ (const Matrix<T>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) + *(w->data + j);
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator+ (const Matrix<U>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<ctype<U>> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) + static_cast<ctype<U>>(*(w->data + j));
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<T> operator+= (const T& val) {
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += val;
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator+= (const U& val) {
        T temp = static_cast<T>(val);
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += temp;
            }
        }
        return *this;
    }

    Matrix<T> operator+= (const Matrix<T>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += *(w->data + j);
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator+= (const Matrix<U>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) += static_cast<T>(*(w->data + j));
            }
        }
        return *this;
    }

    Matrix<T> operator- () const {
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = - *(v->data + j);
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<T> operator- (const T& val) const {
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) - val;
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    friend Matrix<T> operator- (const T& val, const Matrix<T>& m) {
        Vector<T> new_data[m.a];
        for (int i = 0; i < m.a; i++) {
            T temp[m.b];
            auto v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = val - *(v->data + j);
            }
            Vector<T> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(m.a, m.b, new_data);
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator- (const U& val) const {
        Vector<ctype<U>> new_data[this->a];
        auto c = static_cast<ctype<U>>(val);
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) - c;
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        return result;
    }

    template <typename U>
    friend Matrix<ctype<U>> operator- (const U& val, const Matrix<T>& m) {
        Vector<ctype<U>> new_data[m.a];
        auto c = static_cast<ctype<U>>(val);
        for (int i = 0; i < m.a; i++) {
            ctype<U> temp[m.b];
            auto v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = c - static_cast<ctype<U>>(*(*v->data + j));
            }
            Vector<ctype<U>> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(m.a, m.b, new_data);
        return result;
    }

    Matrix<T> operator- (const Matrix<T>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) - *(w->data + j);
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator- (const Matrix<U>& m) const {
        if ((this->a != m.a) and (this->b != m.b)) throw DimensionError();
        Vector<ctype<U>> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) - static_cast<ctype<U>>(*(w->data + j));
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<T> operator-= (const T& val) {
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= val;
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator-= (const U& val) {
        T temp = static_cast<T>(val);
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= temp;
            }
        }
        return *this;
    }

    Matrix<T> operator-= (const Matrix<T>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= *(w->data + j);
            }
        }
        return *this;
    }

    template <typename U>
    Matrix<T> operator-= (const Matrix<U>& m) {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
            auto w = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(v->data + j) -= static_cast<T>(*(w->data + j));
            }
        }
        return *this;
    }

    Matrix<T> operator* (const T& val) const {
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = *(v->data + j) * val;
            }
            Vector<T> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    friend Matrix<T> operator* (const T& val, const Matrix<T>& m) {
        Vector<T> new_data[m.a];
        for (int i = 0; i < m.a; i++) {
            T temp[m.b];
            auto v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                temp[j] = val * *(v->data + j);
            }
            Vector<T> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<T> result(m.a, m.b, new_data);
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator* (const U& val) const {
        Vector<ctype<U>> new_data[this->a];
        auto c = static_cast<ctype<U>>(val);
        for (int i = 0; i < this->a; i++) {
            ctype<U> temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v->data + j)) * c;
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        return result;
    }

    template <typename U>
    friend Matrix<ctype<U>> operator* (const U& val, const Matrix<T>& m) {
        Vector<ctype<U>> new_data[m.a];
        auto c = static_cast<ctype<U>>(val);
        for (int i = 0; i < m.a; i++) {
            ctype<U> temp[m.b];
            auto v = *(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                *(temp + j) = c * static_cast<ctype<U>>(*(v->data + j));
            }
            Vector<ctype<U>> v_temp(m.b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(m.a, m.b, new_data);
        return result;
    }

    Matrix<T> operator* (const Matrix<T>& m) const {
        if (this->b != m.a) throw DimensionError();
        Vector<T> new_data[this->a]; // m.b is the length of each vector

        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i); // pointer to basis vector copied, ~%2.5 faster for high dimensional matrices.
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
        return result;
    }

    template <typename U>
    Matrix<ctype<U>> operator* (const Matrix<U>& m) const {
        if (this->b != m.a) throw DimensionError();
        Vector<ctype<U>> new_data[this->a]; // m.b is the length of each vector

        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i); // basis vector copied
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
        return result;
    }

    Vector<T> operator* (const Vector<T>& v) const {
        if (this->b != v.length) throw DimensionError();
        T temp[this->a];
        auto v_data = v.data;
        for (int i = 0; i < this->a; i++) {
            auto w = *(this->data->data + i);
            T sum = 0;
            for (int j = 0; j < this->b; j++) {
                sum += *(w->data + j) * *(v_data + j);
            }
            *(temp + i) = sum;
        }
        Vector<T> result(this->b, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator* (const Vector<U>& v) const {
        if (this->b != v.length) throw DimensionError();
        ctype<U> temp[this->a];
        for (int i = 0; i < this->a; i++) {
            auto w = *(this->data->data + i);
            ctype<U> sum = 0;
            for (int j = 0; j < this->b; j++) {
                sum += *(w->data + j) * *(v.data + j);
            }
            *(temp + i) = sum;
        }
        Vector<ctype<U>> result(this->b, temp);
        return result;
    }

    template <typename U>
    Matrix<double> operator/ (const U& val) const {
        Vector<double> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            double temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<double>(*(v->data + j)) / val;
            }
            Vector<double> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<double> result(this->a, this->b, new_data);
        return result;
    }

    bool operator== (const Matrix<T>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto a = *(this->data->data + i);
            auto b = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(a->data + j) != *(b->data + j)) return false;
            }
        }
        return true;
    }

    template <typename U>
    bool operator== (const Matrix<U>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto a = *(this->data->data + i);
            auto b = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(a->data + j) != *(b->data + j)) return false;
            }
        }
        return true;
    }

    bool operator!= (const Matrix<T>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto a = *(this->data->data + i);
            auto b = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(a->data + j) == *(b->data + j)) return false;
            }
        }
        return true;
    }

    template <typename U>
    bool operator!= (const Matrix<U>& m) const {
        if ((this->a != m.a) or (this->b != m.b)) throw DimensionError();
        for (int i = 0; i < this->a; i++) {
            auto a = *(this->data->data + i);
            auto b = *(m.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(a->data + j) == *(b->data + j)) return false;
            }
        }
        return true;
    }

    [[nodiscard]] Matrix<int> toInt() const {
        Vector<int> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            int temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<int>(*(v->data + j));
            }
            Vector<int> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<int> result(this->a, this->b, new_data);
        return result;
    }

    [[nodiscard]] Matrix<float> toFloat() const {
        Vector<float> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            float temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<float>(*(v->data + j));
            }
            Vector<float> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<float> result(this->a, this->b, new_data);
        return result;
    }

    [[nodiscard]] Matrix<double> toDouble() const {
        Vector<double> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            double temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<double>(*(v->data + j));
            }
            Vector<double> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<double> result(this->a, this->b, new_data);
        return result;
    }

    [[nodiscard]] Matrix<bool> toBool() const {
        Vector<bool> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            bool temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<bool>(*(v->data + j));
            }
            Vector<bool> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<bool> result(this->a, this->b, new_data);
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
        return result;
    }

    [[nodiscard]] Matrix<T> cumsum() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }

        Vector<T> list[this->a];
        T remainder = 0;
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
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
        return result;
    }

    [[nodiscard]] T sum() const {
        if (this->a == 0) return 0;
        T sum = 0;
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
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
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (i == j) *(temp + j) = *(v->data + j);
                else *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    [[nodiscard]] Matrix<T> getUpper() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }
        if (this->a != this->b) throw DimensionError();
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (i < j) *(temp + j) = *(v->data + j);
                else *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    [[nodiscard]] Matrix<T> getLower() const {
        if (this->a == 0) {
            Matrix<T> result;
            return result;
        }
        if (this->a != this->b) throw DimensionError();
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            auto v = *(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (i > j) *(temp + j) = *(v->data + j);
                else *(temp + j) = 0;
            }
            *(new_data + i) = Vector<T>(this->b, temp);
        }
        Matrix<T> result(this->a, this->b, new_data);
        return result;
    }

    T trace() const {
        if (this->a == 0) return 0;
        if (this->a != this->b) throw DimensionError();
        T mul = 1;
        for (int i = 0; i < this->a; i++) {
            mul *= *((*(this->data->data + i))->data + i);
        }
        return mul;
    }

    T sumDiagonal() const {
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
        for (int i = 0; i < this->a; i++) {
            auto v = *(this->data->data + i);
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
        auto copy_matrix = this->toDouble();

        // Bubble sort
        for (int i = 0; i < this->a; i++) {
            for (int j = 0; j < this->a - i - 1; j++) {
                if ((**(copy_matrix.data->data + j)).__pivot() < (**(copy_matrix.data->data + j + 1)).__pivot()) {
                    temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                    *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                    *(copy_matrix.data->data + j + 1) = temp;
                }
            }
        }

        for (int i = 0; i < this->a; i++) {
            v = *(copy_matrix.data->data + i);
            for (int j = 0; j < this->b; j++) {
                if (*(v->data + j) != 0) {
                    first = *(v->data + j);
                    first_index = j;
                    break;
                }
            }
            for (int r = i + 1; r < this->a; r++) {
                w = *(copy_matrix.data->data + r); // pointer to vector
                rows_first = *(w->data + first_index);
                if (abs(rows_first - 0) < lowlimit) continue;
                factor = rows_first / first;
                if (factor > highlimit) continue;
                *w -= factor * *v;
            }
        }

        for (int i = 0; i < this->a; i++) {
            for (int j = 0; j < this->a - i - 1; j++) {
                if ((**(copy_matrix.data->data + j)).__pivot() < (**(copy_matrix.data->data + j + 1)).__pivot()) {
                    temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                    *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                    *(copy_matrix.data->data + j + 1) = temp;
                }
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
            auto copy_matrix = this->toDouble();

            // Bubble sort
            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((**(copy_matrix.data->data + j)).__pivot() < (**(copy_matrix.data->data + j + 1)).__pivot()) {
                        temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                        *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                        *(copy_matrix.data->data + j + 1) = temp;
                        sign = !sign; // Each swap changes the sign of the determinant
                    }
                }
            }

            for (int i = 0; i < this->a; i++) {
                v = *(copy_matrix.data->data + i);
                for (int j = 0; j < this->b; j++) {
                    if (*(v->data + j) != 0) {
                        first = *(v->data + j);
                        first_index = j;
                        break;
                    }
                }
                for (int r = i + 1; r < this->a; r++) {
                    w = *(copy_matrix.data->data + r); // pointer to vector
                    rows_first = *(w->data + first_index);
                    if (abs(rows_first - 0) < lowlimit) continue;
                    factor = rows_first / first;
                    if (factor > highlimit) continue;
                    *w -= factor * *v;
                }
            }

            for (int i = 0; i < this->a; i++) {
                for (int j = 0; j < this->a - i - 1; j++) {
                    if ((**(copy_matrix.data->data + j)).__pivot() < (**(copy_matrix.data->data + j + 1)).__pivot()) {
                        temp = *(copy_matrix.data->data + j); // a pointer, we will just swap the pointers
                        *(copy_matrix.data->data + j) = *(copy_matrix.data->data + j + 1);
                        *(copy_matrix.data->data + j + 1) = temp;
                        sign = !sign;
                    }
                }
            }

            if (sign) return copy_matrix.trace();
            return -copy_matrix.trace();
        }
    }
};

#endif //VECTOR_CPP_MATRIX_H
