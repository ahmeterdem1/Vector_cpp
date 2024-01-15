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

float randf() {
    float first = rand();
    float second = rand();
    while (second < first) {
        second = rand();
    }
    return first / second;
}

int randint(const int& a, const int& b) {
    if (b < a) throw RangeError();
    auto a_f = float(a);
    auto b_f = float(b);

    if (a_f < 0) {
        float b_p = b_f - a_f;
        float val = randf() * b_p;
        while (val > b_p) {
            val = randf() * b_p;
        }
        return val + a_f;
    }
    float val = (randf() * b_f) + a_f;
    while (val > b_f) {
        val = (randf() * b_f) + a_f;
    }
    return int(val);
}

float randfloat(const float& a, const float& b) {
    if (b <= a) throw RangeError();
    if (a < 0) {
        float b_p = b - a;
        float val = randf() * b_p;
        while (val > b_p) {
            val = randf() * b;
        }
        return val + a;
    }
    float val = (randf() * b) + a;
    while (val > b) {
        val = (randf() * b) + a;
    }
    return val;
}

double randdouble(const double& a, const double& b) {
    if (b <= a) throw RangeError();
    if (a < 0) {
        double b_p = b - a;
        double val = randf() * b_p;
        while (val > b_p) {
            val = randf() * b;
        }
        return val + a;
    }
    double val = (randf() * b) + a;
    while (val > b) {
        val = (randf() * b) + a;
    }
    return val;
}

bool randbool() {
    if (rand() % 2) return true;
    return false;
}

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

        if (this->size == 0) {
            this->data = new T[2];
            *this->data = in;
            this->size = 2;
            this->length = 1;
            return;
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
        if (this->length == 0) throw IndexError();
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

    template <typename U>
    Vector<double> operator/ (const Vector<U>& v) {
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

    Vector<int> toInt() const {
        int temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<int>(*(this->data + i));
        }
        Vector<int> result(this->length, temp);
        return result;
    }

    Vector<float> toFloat() const {
        float temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<float>(*(this->data + i));
        }
        Vector<float> result(this->length, temp);
        return result;
    }

    Vector<double> toDouble() const {
        double temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<double>(*(this->data + i));
        }
        Vector<double> result(this->length, temp);
        return result;
    }

    Vector<bool> toBool() const {
        bool temp[this->length];
        for (int i = 0; i < this->length; i++) {
            *(temp + i) = static_cast<bool>(*(this->data + i));
        }
        Vector<bool> result(this->length, temp);
        return result;
    }
};

#endif //VECTOR_CPP_VECTORGEBRA_H