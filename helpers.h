//
// Created by AHMET ERDEM on 19.01.2024.
//

#ifndef VECTOR_CPP_HELPERS_H
#define VECTOR_CPP_HELPERS_H

#define PI 3.1415926535
#define E 2.718281828459
#define DtoR 0.01745329252
#define in :
#define undefined NaN(2)

#include "exceptions.h"
#include <ostream>

template <typename T>
class Range {
public:
    T step = 1;
    T first = 0;
    T last = 0;

    class Iterator {
        T* current = nullptr;
        T step = 1;
    public:
        explicit Iterator(T* inn, const T& s) {
            this->current = inn;
            this->step = s;
        }

        T& operator* () const {
            return *this->current;
        }

        bool operator!= (const Iterator& i) const {
            // lol
            return *this->current <= *i.current;
        }

        Iterator& operator++ () {
            *(this->current) += this->step;
            return *this;
        }
    };

    Range() = default;

    explicit Range(const T& _last) {
        this->last = _last;
    }

    Range(const T& _first, const T& _last) {
        if (_first > _last) throw RangeError();
        this->first = _first;
        this->last = _last;
    }

    Range(const T& _first, const T& _last, const T& _step) {
        if (_first > _last) throw RangeError();
        this->first = _first;
        this->last = _last;
        this->step = _step;
    }

    Iterator begin() {
        return Iterator(&(this->first), this->step);
    }

    Iterator end() {
        return Iterator(&(this->last), this->step);
    }
};

class NaN {
public:
    unsigned int state = 1;

    NaN() = default;

    explicit NaN (const unsigned int& s) {
        if (s == 0) this->state = 0;
        else if (s == 1) this->state = 1;
        else this->state = 2;
    }

    friend std::ostream& operator<< (std::ostream& o, const NaN& n) {
        if (n.state == 0) o << "Infinity(-)";
        else if (n.state == 1) o << "Infinity(+)";
        else o << "Undefined";
        return o;
    }

    NaN operator+ (const NaN& n) const {
        if (this->state == 2 or n.state == 2) return undefined;
        if (this->state ^ n.state) return undefined;
        return NaN(this->state);
    }

    template <typename T>
    NaN operator+ (const T& anything) const {
        return NaN(this->state);
    }

    template <typename T>
    friend NaN operator+ (const T& anything, const NaN& n) {
        return NaN(n.state);
    }

    NaN operator- (const NaN& n) const {
        if (this->state == 2 or n.state == 2) return undefined;
        if (!(this->state ^ n.state)) return undefined;
        return NaN(this->state);
    }

    template <typename T>
    NaN operator- (const T& anything) const {
        return NaN(this->state);
    }

    template <typename T>
    friend NaN operator- (const T& anything, const NaN& n) {
        if (n.state == 2) return undefined;
        if (n.state == 1) return NaN(0);
        return NaN(1);
    }

    NaN operator* (const NaN& n) const {
        if (this->state == 2 or n.state == 2) return undefined;
        return NaN(this->state and n.state);
    }

    template <typename T>
    NaN operator* (const T& anything) const {
        if (this->state == 2) return undefined;
        if (anything > 0) return NaN(this->state);
        if (anything < 0) {
            if (this->state == 1) return NaN(0);
            return NaN(1);
        }
        if (anything == 0) return undefined;
    }

    template <typename T>
    friend NaN operator* (const T& anything, const NaN& n) {
        if (n.state == 2) return undefined;
        if (anything > 0) return NaN(n.state);
        if (anything < 0) {
            if (n.state == 1) return NaN(0);
            return NaN(1);
        }
        if (anything == 0) return undefined;
    }

    NaN operator/ (const NaN& n) const {
        return undefined;
    }

    template <typename T>
    NaN operator/ (const T& anything) const {
        if (this->state == 2) return undefined;
        if (anything > 0) return NaN(this->state);
        if (anything < 0) {
            if (this->state == 1) return NaN(0);
            return NaN(1);
        }
        if (anything == 0) return undefined;
    }

    template <typename T>
    friend NaN operator/ (const T& anything, const NaN& n) {
        return undefined;
    }

    template <typename T>
    NaN operator== (const T& anything) const {
        return undefined;
    }

    template <typename T>
    NaN operator!= (const T& anything) const {
        return undefined;
    }

    bool operator> (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return true;
        return false;
    }

    template <typename T>
    bool operator> (const T& anything) const {
        if (this->state == 2 or this->state == 0) return false;
        return true;
    }

    bool operator>= (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return true;
        return false;
    }

    template <typename T>
    bool operator>= (const T& anything) const {
        if (this->state == 2 or this->state == 0) return false;
        return true;
    }

    bool operator< (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return false;
        return true;
    }

    template <typename T>
    bool operator< (const T& anything) const {
        if (this->state == 2 or this->state == 0) return true;
        return false;
    }

    bool operator<= (const NaN& n) const {
        if (this->state == 2 or n.state == 2 or !(this->state ^ n.state)) return false;
        if (this->state) return false;
        return true;
    }

    template <typename T>
    bool operator<= (const T& anything) const {
        if (this->state == 2 or this->state == 0) return true;
        return false;
    }
};

NaN Infinity(const bool& sign = true) {
    if (sign) return NaN(1);
    return NaN(0);
}

NaN Undefined() {
    return undefined;
}

#endif //VECTOR_CPP_HELPERS_H
