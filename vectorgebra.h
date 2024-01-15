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

    void resize() {
        int start = 1;
        while (start < this->length) {
            start *= 2;
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

    void append(const T& in) {
        if (this->size == 0) {
            this->data = new T[2];
            *this->data = in;
            this->size = 2;
            this->length = 1;
            return;
        }

        if (this->length < this->size) {
            this->data[this->length] = in;
            this->length++;
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
        return;
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

    T dot (const Vector<T>& v) const {
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

    static double determinant(const int& amount, const Vector<T>* v_list) {
        if (amount == 0) throw RangeError();
        for (int i = 0; i < amount; i++) if (amount != (v_list + i)->length) throw DimensionError();
        if (amount == 1) return 1 / *(v_list->data);
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
};

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

    void resize() {
        this->data->resize();
    }

    void reverse() {
        this->data->reverse();
    }

    Vector<T> operator[] (int index) const {
        if (this->a == 0) throw IndexError();
        if (index < 0) index += this->a;
        if (index < 0 or index >= this->a) throw IndexError();
        return **(this->data->data + index);
    }

    Matrix<T> transpose() const {
        Vector<T> new_data[this->b];
        for (int i = 0; i < this->b; i++) {
            T temp[this->a];
            for (int j = 0; j < this->a; j++) {
                temp[j] = *((*(this->data->data + j))->data + i);
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
            for (int j = 0; j < this->b; j++) {
                temp[j] = *((*(this->data->data + i))->data + j) + val;
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
            for (int j = 0; j < m.b; j++) {
                temp[j] = *((*(m.data->data + i))->data + j) + val;
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
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*((*(this->data->data + i))->data + j)) + c;
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
            for (int j = 0; j < m.b; j++) {
                temp[j] = static_cast<ctype<U>>(*((*(m.data->data + i))->data + j)) + c;
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
            for (int j = 0; j < this->b; j++) {
                temp[j] = *((*(this->data->data + i))->data + j) + *((*(m.data->data + i))->data + j);
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
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*((*(this->data->data + i))->data + j)) + static_cast<ctype<U>>(*((*(m.data->data + i))->data + j));
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<T> operator- (const T& val) const {
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            for (int j = 0; j < this->b; j++) {
                temp[j] = *((*(this->data->data + i))->data + j) - val;
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
            for (int j = 0; j < m.b; j++) {
                temp[j] = val - *((*(m.data->data + i))->data + j);
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
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*((*(this->data->data + i))->data + j)) - c;
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
            for (int j = 0; j < m.b; j++) {
                temp[j] = c - static_cast<ctype<U>>(*((*(m.data->data + i))->data + j));
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
            for (int j = 0; j < this->b; j++) {
                temp[j] = *((*(this->data->data + i))->data + j) - *((*(m.data->data + i))->data + j);
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
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*((*(this->data->data + i))->data + j)) - static_cast<ctype<U>>(*((*(m.data->data + i))->data + j));
            }
            Vector<ctype<U>> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<ctype<U>> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<T> operator* (const T& val) const {
        Vector<T> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            T temp[this->b];
            for (int j = 0; j < this->b; j++) {
                temp[j] = *((*(this->data->data + i))->data + j) * val;
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
            for (int j = 0; j < m.b; j++) {
                temp[j] = val * *((*(m.data->data + i))->data + j);
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
            auto v = **(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                temp[j] = static_cast<ctype<U>>(*(v.data + j)) * c;
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
            auto v = **(m.data->data + i);
            for (int j = 0; j < m.b; j++) {
                *(temp + j) = c * static_cast<ctype<U>>(*(v.data + j));
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
            auto v = **(this->data->data + i); // basis vector copied, faster for higher dimensional operations
            T temp[m.b];
            for (int j = 0; j < m.b; j++) {
                T sum = 0;
                for (int k = 0; k < m.a; k++) {
                    sum += *(v.data + k) * *((*(m.data->data + k))->data + j);
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
            auto v = **(this->data->data + i); // basis vector copied
            ctype<U> temp[m.b];
            for (int j = 0; j < m.b; j++) {
                ctype<U> sum = 0;
                for (int k = 0; k < m.a; k++) {
                    sum += static_cast<ctype<U>>(*(v.data + k)) * static_cast<ctype<U>>(*((*(m.data->data + k))->data + j));
                }
                *(temp + j) = sum;
            }
            Vector<ctype<U>> to_add(m.b, temp);
            *(new_data + i) = to_add;
        }
        Matrix<ctype<U>> result(this->a, m.b, new_data);
        return result;
    }

    template <typename U>
    Matrix<double> operator/ (const U& val) const {
        Vector<double> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            double temp[this->b];
            auto v = **(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<double>(*(v.data + j)) / val;
            }
            Vector<double> v_temp(this->b, temp);
            *(new_data + i) = v_temp;
        }
        Matrix<double> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<int> toInt() const {
        Vector<int> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            int temp[this->b];
            auto v = **(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<int>(*(v.data + j));
            }
            Vector<int> v_temp(this->b, temp);
            *(temp + i) = v_temp;
        }
        Matrix<int> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<float> toFloat() const {
        Vector<float> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            float temp[this->b];
            auto v = **(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<float>(*(v.data + j));
            }
            Vector<float> v_temp(this->b, temp);
            *(temp + i) = v_temp;
        }
        Matrix<float> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<double> toDouble() const {
        Vector<double> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            double temp[this->b];
            auto v = **(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<double>(*(v.data + j));
            }
            Vector<double> v_temp(this->b, temp);
            *(temp + i) = v_temp;
        }
        Matrix<double> result(this->a, this->b, new_data);
        return result;
    }

    Matrix<bool> toBool() const {
        Vector<bool> new_data[this->a];
        for (int i = 0; i < this->a; i++) {
            bool temp[this->b];
            auto v = **(this->data->data + i);
            for (int j = 0; j < this->b; j++) {
                *(temp + j) = static_cast<bool>(*(v.data + j));
            }
            Vector<bool> v_temp(this->b, temp);
            *(temp + i) = v_temp;
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
                else *(temp + j) = 1;
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
};

#endif //VECTOR_CPP_VECTORGEBRA_H