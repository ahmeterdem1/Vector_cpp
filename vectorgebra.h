#ifndef VECTOR_CPP_VECTORGEBRA_H
#define VECTOR_CPP_VECTORGEBRA_H

#include <ostream>
#include <type_traits>

class IndexError : public std::exception {
public:
    IndexError() = default;

    const char* what() {
        return "IndexError: Incorrect indexing";
    }
};

class DimensionError : public std::exception {
public:
    DimensionError() = default;

    const char* what() {
        return "DimensionError: Incorrect dimensions for the operation";
    }
};

class ZeroDivisionError : public std::exception {
public:
    ZeroDivisionError() = default;

    const char* what() {
        return "ZeroDivisionError: Division by 0";
    }
};

class RangeError : public std::exception {
public:
    RangeError() = default;

    const char* what() {
        return "RangeError: Argument(s) out of range";
    }
};

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
        auto temp_size = float(data_size);
        while (temp_size > 1) {
            temp_size /= 2;
            power *= 2;
        }

        T *in = new T[power];

        for (int i = 0; i < data_size; i++) {
            in[i] = data_array[i];
        }
        this->length = data_size;
        this->size = power;
        this->data = in;
    }

    unsigned int getsize() {
        return this->size;
    }

    unsigned int getlength() {
        return this->length;
    }

    T operator[] (int index) {
        if (index < 0) index += this->length;
        if (index < 0) throw IndexError();
        return *(this->data + index);
    }

    friend std::ostream& operator<< (std::ostream& o, Vector a) {
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

    Vector append(const T& in) {
        if (this->length < this->size) {
            this->data[this->length] = in;
            this->length++;
            return *this;
        }

        T *new_data = new T[2 * this->size];
        auto* temp = this->data;
        std::move(temp, temp + this->size, new_data);
        *(new_data + this->size) = in;
        temp = nullptr;
        this->data = new_data;
        this->size *= 2;
        this->length += 1;
        return *this;
    }

    T pop(int index = -1) {
        if (index < 0) index += this->length;
        if (index < 0) throw IndexError();

        T r_value = *(this->data + index);

        for (int i = index; i < this->length - 1; i++) {
            *(this->data + i) = *(this->data + i + 1);
        }
        this->length--;
        return r_value;
    }

    Vector<T> clear() {
        if (this->data != nullptr) {
            delete[] this->data;
            this->data = nullptr;
            this->length = 0;
        }
        return *this;
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
    Vector<ctype<U>> operator+ (Vector<U> v) {
        if (this->length != v.length) throw DimensionError();

        ctype<U> temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) + static_cast<ctype<U>>(*(v.data + i));
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator+ (U a) {
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
    friend Vector<ctype<U>> operator+ (U a, Vector<T> v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = static_cast<ctype<U>>(*(v.data + i)) + val;
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
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
    Vector<ctype<U>> operator- (Vector<U> v) {
        if (this->length != v.length) throw DimensionError();

        ctype<U> temp[this->length];
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) - static_cast<ctype<U>>(*(v.data + i));
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    template <typename U>
    Vector<ctype<U>> operator- (U a) {
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
    friend Vector<ctype<U>> operator- (U a, Vector<T> v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = val - static_cast<ctype<U>>(*(v.data + i));
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
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
    ctype<U> operator* (Vector<U> v) {
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
    Vector<ctype<U>> operator* (U a) {
        ctype<U> temp[this->length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<ctype<U>>(*(this->data + i)) * val;
        }
        Vector<ctype<U>> result(this->length, temp);
        return result;
    }

    template <typename U>
    friend Vector<ctype<U>> operator* (U a, Vector<T> v) {
        ctype<U> temp[v.length];
        auto val = static_cast<ctype<U>>(a);
        for (int i = 0; i < v.length; i++) {
            temp[i] = static_cast<ctype<U>>(*(v.data + i)) * val;
        }
        Vector<ctype<U>> result(v.length, temp);
        return result;
    }

    template <typename U>
    Vector<double> operator/ (U a) {
        if (a == 0) throw ZeroDivisionError();
        double temp[this->length];
        auto val = static_cast<double>(a);
        for (int i = 0; i < this->length; i++) {
            temp[i] = static_cast<double>(*(this->data + i)) / val;
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    T dot (const Vector<T>& v) const {
        if (this->length != v.length) throw DimensionError();
        T sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += *(this->data + i) * *(v.data + i);
        }
        return sum;
    }

    template <typename U>
    ctype<U> dot (Vector<U> v) {
        if (this->length != v.length) throw DimensionError();
        ctype<U> sum = 0;
        for (int i = 0; i < this->length; i++) {
            sum += static_cast<ctype<U>>(*(this->data + i)) * static_cast<ctype<U>>(*(v.data + i));
        }
        return sum;
    }

    static Vector<T> zero(const int& dim) {
        if (dim < 0) throw RangeError();
        T temp[dim];
        for (int i = 0; i < dim; i++) {
            temp[i] = 0;
        }
        Vector<T> result(dim, temp);
        return result;
    }

    static Vector<T> one(const int& dim) {
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
        // I am going to prevent the extra function calls no matter what.
        double temp[this->length];
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
};

#endif //VECTOR_CPP_VECTORGEBRA_H