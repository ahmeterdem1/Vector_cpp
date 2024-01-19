//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_VECTOR_H
#define VECTOR_CPP_VECTOR_H

#include <type_traits>
#include "functions.h"

template <typename T>
class Vector {
    unsigned int size = 0;
    template <typename U>
    using ctype = typename std::common_type<T, U>::type;

public:
    unsigned int length = 0;
    T *data = nullptr;

    Vector() = default;

    Vector(unsigned int data_size, T *data_array) {
        int power = 1;
        if (data_size < 64) {
            auto temp_size = float(data_size);
            while (temp_size > 1) {
                temp_size /= 2;
                power *= 2;
            }
        } else {
            power = 64 * floor(data_size / 64) + 64;
        }

        T *inn= new T[power];

        for (int i = 0; i < data_size; i++) {
            inn[i] = data_array[i];
        }
        this->length = data_size;
        this->size = power;
        this->data = inn;
    }

    unsigned int getsize() {
        return this->size;
    }

    unsigned int getlength() {
        return this->length;
    }

    void resize() {
        unsigned int start = 1;
        if (this->length < 64) {
            while (start < this->length) {
                start *= 2;
            }
        } else {
            start = 64 * floor(this->length / 64) + 64;
        }
        if (start == this->size) return;
        auto* temp = this->data;
        T* new_data = new T[start];
        std::move(temp, temp + this->length, new_data);
        this->size = start;
    }

    T operator[] (int index) {
        if (this->length == 0) throw IndexError();
        if (index < 0) index += this->length;
        if (index < 0 or index >= this->length) throw IndexError();
        return *(this->data + index);
    }

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

    void append(const T& inn) {
        if (this->size == 0) {
            this->data = new T[2];
            *this->data = inn;
            this->size = 2;
            this->length = 1;
            return;
        }

        if (this->length < this->size) {
            this->data[this->length] = inn;
            this->length++;
            return;
        }

        if (this->size < 64) {
            // Resizing is exponential
            T *new_data = new T[2 * this->size];
            auto *temp = this->data;
            std::move(temp, temp + this->size, new_data);
            *(new_data + this->size) = inn;
            temp = nullptr;
            this->data = new_data;
            this->size *= 2;
            this->length += 1;
        } else {
            // Resizing is linear, 64 places per increase
            T *new_data = new T[this->size + 64];
            auto *temp = this->data;
            std::move(temp, temp + this->size, new_data);
            *(new_data + this->size) = inn;
            temp = nullptr;
            this->data = new_data;
            this->size += 64;
            this->length += 1;
        }
    }

    T pop(int index = -1) {
        if (this->length == 0) throw IndexError();
        if (index < 0) index += this->length;
        if (index < 0 or index >= this->length) throw IndexError();

        T r_value = *(this->data + index);

        for (int i = index; i < this->length - 1; i++) {
            *(this->data + i) = *(this->data + i + 1);
        }
        this->length--;
        return r_value;
    }

    void insert(int index, T val) {
        if (this->length == 0) {
            T* new_data = new T[2];
            *new_data = val;
            this->size = 2;
            this->length = 1;
            this->data = new_data;
            return;
        }
        if (index < 0) index += this->length;
        if (index < 0) throw IndexError();
        if (this->size - this->length > 0) {
            auto temp = *(this->data + index);
            *(this->data + index) = val;
            T temp2;
            for (int i = index + 1; i < this->length; i++) {
                temp2 = *(this->data + i);
                *(this->data + i) = temp;
                temp = temp2;
            }
            *(this->data + this->length) = temp;
        } else if (this->size < 64) {
            T *new_data = new T[2 * this->size];
            auto *temp = this->data;
            std::move(temp, temp + index, new_data);
            *(new_data + index) = val;
            for (int i = index + 1; i < this->length; i++) {
                *(new_data + i) = *(temp + i - 1);
            }
            this->size *= 2;
            this->data = new_data;
        } else {
            T *new_data = new T[this->size + 64];
            auto *temp = this->data;
            std::move(temp, temp + index, new_data);
            *(new_data + index) = val;
            for (int i = index + 1; i < this->length; i++) {
                *(new_data + i) = *(temp + i - 1);
            }
            this->size += 64;
            this->data = new_data;
        }
        this->length += 1;
    }

    Vector<T> clear() {
        if (this->data != nullptr) {
            delete[] this->data;
            this->data = nullptr;
            this->length = 0;
        }
        return *this;
    }

    int __pivot(const double& limiter = 0.0000000000000000001) {
        if (this->length == 0) return 0;
        for (int i = 0; i < this->length; i++) {
            if (abs(*(this->data + i) - 0) < limiter) return i;
        }
        return this->length;
    }

    Vector<T> operator+ (const T& a) const {
        T temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = *(this->data + i) + a;
        }
        Vector<T> result(this->length, temp);
        return result;
    }

    Vector<T> operator+ (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();

        T temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = *(this->data + i) + *(v.data + i);
        }

        Vector<T> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator+ (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();

        ctype<U> temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) + static_cast<ctype<U>>(*(v.data + i));
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator+ (const U& a) const {
        ctype<U> temp[this->length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) + val;
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    friend Vector<T> operator+ (const T& a, const Vector<T>& v) {
        T temp[v.length];
        for (int i = 0; i < v.length; i++) {
            temp[i] = *(v.data + i) + a;
        }
        Vector<T> result(v.length, temp);
        return result;
    }

    template <typename U>
    friend Vector<ctype<U>> operator+ (const U& a, const Vector<T>& v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = static_cast<ctype<U>>(*(v.data + i)) + val;
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
    }

    Vector<T> operator+= (const T& a) {
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) += a;
        }
        return *this;
    }

    template <typename U>
    Vector<T> operator+= (const U& a) {
        T temp = static_cast<T>(a);
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) += temp;
        }
        return *this;
    }

    Vector<T> operator+= (const Vector<T>& v) {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) += *(v.data + i);
        }
        return *this;
    }

    template <typename U>
    Vector<T> operator+= (const Vector<U>& v) {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) += static_cast<T>(*(v.data + i));
        }
        return *this;
    }

    Vector<T> operator- (const T& a) const {
        T temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = *(this->data + i) - a;
        }
        Vector<T> result(this->length, temp);
        return result;
    }

    Vector<T> operator- (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();

        T temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = *(this->data + i) - *(v.data + i);
        }

        Vector<T> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator- (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();

        ctype<U> temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) - static_cast<ctype<U>>(*(v.data + i));
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator- (const U& a) const {
        ctype<U> temp[this->length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) - val;
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    friend Vector<T> operator- (const T& a, const Vector<T>& v) {
        T temp[v.length];
        for (int i = 0; i < v.length; i++) {
            temp[i] = a - *(v.data + i);
        }
        Vector<T> result(v.length, temp);
        return result;
    }

    template <typename U>
    friend Vector<ctype<U>> operator- (const U& a, const Vector<T>& v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = val - static_cast<ctype<U>>(*(v.data + i));
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
    }

    Vector<T> operator-= (const T& a) {
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) += a;
        }
        return *this;
    }

    template <typename U>
    Vector<T> operator-= (const U& a) {
        T temp = static_cast<T>(a);
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) += temp;
        }
        return *this;
    }

    Vector<T> operator-= (const Vector<T>& v) {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) -= *(v.data + i);
        }
        return *this;
    }

    template <typename U>
    Vector<T> operator-= (const Vector<U>& v) {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) -= static_cast<T>(*(v.data + i));
        }
        return *this;
    }

    Vector<T> operator* (const T& a) const {
        T temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = *(this->data + i) * a;
        }
        Vector<T> result(this->length, temp);
        return result;
    }

    T operator* (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        T sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += *(this->data + i) * *(v.data + i);
        }
        return sum;
    }

    template <typename U>
    ctype<U> operator* (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        ctype<U> sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += static_cast<ctype<U>>(*(this->data + i)) * static_cast<ctype<U>>(*(v.data + i));
        }
        return sum;
    }

    friend Vector<T> operator* (const T& a, const Vector<T>& v) {
        T temp[v.length];
        for (int i = 0; i < v.length; i++) {
            temp[i] = *(v.data + i ) * a;
        }
        Vector<T> result(v.length, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator* (const U& a) const {
        ctype<U> temp[this->length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) * val;
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    template <typename U>
    friend Vector<ctype<U>> operator* (const U& a, const Vector<T>& v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = static_cast<ctype<U>>(*(v.data + i)) * val;
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
    }

    Vector<T> operator*= (const T& a) {
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) *= a;
        }
        return *this;
    }

    template <typename U>
    Vector<T> operator*= (const U& a) {
        T temp = static_cast<T>(a);
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) *= temp;
        }
        return *this;
    }

    template <typename U>
    Vector<double> operator/ (const U& a) const {
        if (a == 0) throw ZeroDivisionError();
        double temp[this->length];
        auto val = static_cast<double>(a);
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<double>(*(this->data + i)) / val;
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<double> operator/ (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();

        double temp[this->length];
        for (int i = 0; i < this->length; i++) {
            auto val = static_cast<double>(*(v.data + i));
            if (val == 0) throw ZeroDivisionError();
            temp[i] = static_cast<double>(*(this->data + i)) / val;
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    Vector<T> operator++ () {
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) += 1;
        }
        return *this;
    }

    Vector<T> operator-- () {
        for (int i = 0; i < this->length; i++) {
            *(this->data + i) -= 1;
        }
        return *this;
    }

    bool operator== (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            if (*(this->data + i) != *(v.data + i)) return false;
        }
        return true;
    }

    template <typename U>
    bool operator== (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            if (*(this->data + i) != *(v.data + i)) return false;
        }
        return true;
    }

    bool operator!= (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            if (*(this->data + i) == *(v.data + i)) return false;
        }
        return true;
    }

    template <typename U>
    bool operator!= (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        for (int i = 0; i < this->length; i++) {
            if (*(this->data + i) == *(v.data + i)) return false;
        }
        return true;
    }

    bool operator> (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 > 0;
    }

    template <typename U>
    bool operator> (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 > 0;
    }

    bool operator>= (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 >= 0;
    }

    template <typename U>
    bool operator>= (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 >= 0;
    }

    bool operator< (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 < 0;
    }

    template <typename U>
    bool operator< (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 < 0;
    }

    bool operator<= (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 <= 0;
    }

    template <typename U>
    bool operator<= (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        double sum1 = 0;
        for (int i = 0; i < this->length; i++) {
            sum1 += pow(*(this->data + i), 2) - pow(*(v.data + i), 2);
        }
        return sum1 <= 0;
    }

    [[nodiscard]] T dot (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        T sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += *(this->data + i) * *(v.data + i);
        }
        return sum;
    }

    template <typename U>
    ctype<U> dot (const Vector<U>& v) const {
        if (this->length != v.length) throw DimensionError();
        ctype<U> sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += static_cast<ctype<U>>(*(this->data + i)) * static_cast<ctype<U>>(*(v.data + i));
        }
        return sum;
    }

    static Vector<T> zero(const unsigned int& dim) {
        if (dim < 0) throw RangeError();
        T temp[dim];
        for (int i = 0; i < dim; i++) {
            temp[i] = 0;
        }
        Vector<T> result(dim, temp);
        return result;
    }

    static Vector<T> one(const unsigned int& dim) {
        if (dim < 0) throw RangeError();
        T temp[dim];
        for (int i = 0; i < dim; i++) {
            temp[i] = 1;
        }
        Vector<T> result(dim, temp);
        return result;
    }

    Vector<double> unit() {
        double sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += pow(*(this->data + i), 2);
        }
        if (sum == 0) throw ZeroDivisionError();
        double temp[this->length];
        sum = sqrt(sum, 15);
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<double>(*(this->data + i)) / sum;
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    double len() {
        double sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += pow(*(this->data + i), 2);
        }
        return sqrt(sum);
    }

    template <typename U>
    Vector<double> proj(const Vector<U>& v) {
        if (this->length != v.length) throw DimensionError();

        double temp[this->length];
        double sum = 0;
        double l = 0;
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<double>(*(v.data + i));
            sum += static_cast<double>(*(this->data + i)) * temp[i];
            l += pow(temp[i], 2);
        }
        if (l == 0) throw ZeroDivisionError();
        double factor = sum / l;
        for (int i = 0; i < this->length; i++) {
            temp[i] *= factor;
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    static Vector<double>* spanify(const unsigned int& amount, Vector<double> array[]) {
        // You put in a vector array, it changes the vector array and returns the pointer to it.

        *array = array->unit();
        for (int i = 1; i < amount; i++) {
            for (int j = 0; j < i; j++) {
                *(array + i) -= (array + i)->proj(*(array + j));
            }
            *(array + i) = (array + i)->unit();
        }
        return array;
    }

    static bool doesSpan(const unsigned int& amount, const Vector<double> array[], const double& limiter = 0.0000000001) {
        for (int i = 0; i < amount; i++) {
            for (int j = i + 1; j < amount; j++) {
                if (*(array + i) * *(array + j) > limiter) return false;
            }
        }
        return true;
    }

    static Vector<int> randVint(const int& d, const int& a, const int& b) {
        int temp[d];
        for (int i = 0; i < d; i++) {
            *(temp + i) = randint(a, b);
        }
        Vector<int> result(d, temp);
        return result;
    }

    static Vector<float> randVfloat(const int& d, const float& a, const float& b) {
        float temp[d];
        for (int i = 0; i < d; i++) {
            *(temp + i) = randfloat(a, b);
        }
        Vector<float> result(d, temp);
        return result;
    }

    static Vector<double> randVdouble(const int& d, const double& a, const double& b) {
        double temp[d];
        for (int i = 0; i < d; i++) {
            *(temp + i) = randdouble(a, b);
        }
        Vector<double> result(d, temp);
        return result;
    }

    static Vector<bool> randVbool(const int& d) {
        bool temp[d];
        for (int i = 0; i < d; i++) {
            *(temp + i) = randbool();
        }
        Vector<bool> result(d, temp);
        return result;
    }

    [[nodiscard]] Vector<int> toInt() const {
        int temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<int>(*(this->data + i));
        }
        Vector<int> result(this->length, temp);
        return result;
    }

    [[nodiscard]] Vector<float> toFloat() const {
        float temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<float>(*(this->data + i));
        }
        Vector<float> result(this->length, temp);
        return result;
    }

    [[nodiscard]] Vector<double> toDouble() const {
        double temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<double>(*(this->data + i));
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    [[nodiscard]] Vector<bool> toBool() const {
        bool temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<bool>(*(this->data + i));
        }
        Vector<bool> result(this->length, temp);
        return result;
    }

    static double determinant(const int& amount, const Vector<T>* v_list) {
        if (amount == 0) throw RangeError();
        for (int i = 0; i < amount; i++) if (amount != (v_list + i)->length) throw DimensionError();
        if (amount == 1) return *(v_list->data);
        if (amount == 2) {
            return (*(v_list->data) * *((v_list + 1)->data + 1)) - (*(v_list->data + 1) * *((v_list + 1)->data));
        }
        double sum = 0;
        for (int i = 0; i < amount; i++) {
            Vector<T> temp_list[amount - 1];
            for (int j = 1; j < amount; j++) {
                Vector<T> temp_v;
                for (int k = 0; k < amount; k++) {
                    if (k == i) continue;
                    temp_v.append(*((v_list + j)->data + k));
                }
                *(temp_list + j - 1) = temp_v;
            }
            sum += pow(-1, i) * *(v_list->data + i) * Vector<T>::determinant(amount - 1, temp_list);
        }
        return sum;
    }

    static Vector<double> cross(const int& amount, const Vector<T>* v_list) {
        if (amount == 0) throw RangeError();
        for (int i = 0; i < amount; i++) if (amount + 1 != (v_list + i)->length) throw DimensionError();

        Vector<double> new_v[amount];
        for (int i = 0; i < amount; i++) {
            *(new_v + i) = (v_list + i)->toDouble();
        }

        double values[amount + 1];

        for (int i = 0; i < v_list->length; i++) {
            Vector<double> v_list2[amount];
            for (int j = 1; j < amount + 1; j++) {
                Vector<double> to_add;
                for (int k = 0; k < v_list->length; k++) {
                    if (i == k) continue;
                    to_add.append(*((new_v + j - 1)->data + k));
                }
                *(v_list2 + j - 1) = to_add;
            }
            values[i] = pow(-1, i) * Vector<double>::determinant(amount, v_list2);
        }
        Vector<double> result(amount + 1, values);
        return result;
    }

    void reverse() {
        T *temp = new T[this->size];
        for (int i = 0; i < this->length; i++) {
            temp[this->length - i - 1] = *(this->data + i);
        }
        delete[] this->data;
        this->data = temp;
    }

    Vector<T> cumsum() {
        if (this->length == 0) {
            Vector<T> result;
            return result;
        }

        T temp[this->length];
        *temp = *this->data;
        for (int i = 1; i < this->length; i++) {
            *(temp + i) = *(temp + i - 1) + *(this->data + i);
        }
        Vector<T> result(this->length, temp);
        return result;
    }

    T sum() {
        if (this->length == 0) return 0;
        T sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += *(this->data + i);
        }
        return sum;
    }

    [[nodiscard]] Vector<double> softmax() const {
        if (this->length == 0) {
            Vector<double> result;
            return result;
        }
        double temp[this->length];
        double sum = 0;
        for (int i = 0; i < this->length; i++) {
            double val = e(*(this->data + i));
            sum += val;
            *(temp + i) = val;
        }
        if (sum == 0) throw ZeroDivisionError();
        for (int i = 0; i < this->length; i++) {
            *(temp + i) /= sum;
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    [[nodiscard]] Vector<double> sig(const double& a, const unsigned int& resolution = 15) const {
        if (this->length == 0) {
            Vector<double> result;
            return result;
        }
        double temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = sigmoid(*(this->data + i), a, resolution);
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    [[nodiscard]] Vector<double> relu(const double& cutoff = 0, const double& leak = 0) const {
        if (this->length == 0) {
            Vector<double> result;
            return result;
        }
        double temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = ReLU(*(this->data + i), cutoff, leak);
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    [[nodiscard]] Vector<double> minmax() {
        auto maxima = static_cast<double>(maximum(this->length, this->data));
        auto minima = static_cast<double>(minimum(this->length, this->data));
        double temp[this->length];
        auto diff = maxima - minima;
        if (diff == 0) throw ZeroDivisionError();
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = (*(this->data + i) - minima) / diff;
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<U> map(std::function<U(T)> f) {
        U temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = f(*(this->data + i));
        }
        Vector<U> result(this->length, temp);
        return result;
    }

    Vector<T> filter(std::function<bool(T)> f) {
        T temp[this->length];
        int count = 0;
        for (int i = 0; i < this->length; i++) {
            if (f(*(this->data + i))) {
                *(temp + count) = *(this->data + i);
                count++;
            }
        }
        Vector<T> result(count, temp); // "count" is the length, not this->length
        return result;
    }
};

#endif //VECTOR_CPP_VECTOR_H
