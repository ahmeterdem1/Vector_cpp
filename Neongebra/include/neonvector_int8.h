//
// Created by AHMET ERDEM on 2.07.2024.
//

#pragma once

#ifndef VECTOR_CPP_NEONVECTOR_H
#define VECTOR_CPP_NEONVECTOR_H

#include <arm_neon.h>
#include <algorithm>  // The std::move version that append uses comes from here
#include <ostream>
#include "./exceptions.h"
#include <iostream>
// Important, this->size is NEVER zero for neon supported vectors.
// And, they are ALWAYS a multiple of 64.
// This is for consistency with SIMD operations.

class vector_int8 {
public:

    unsigned int size;
    unsigned int length;
    int8_t* data = nullptr;

    vector_int8();

    vector_int8(unsigned int data_size, int8_t* data);

    // This special constructor is for internal use only
private:
    vector_int8(int8_t* data, unsigned int data_size, unsigned int data_length);

public:

    friend std::ostream& operator<< (std::ostream& o, const vector_int8& a) {
        // This function automatically casts values of self to "int".
        // For all other applications, If you want to print the values,
        // you need to cast them manually. I will not cast any return values.
        o << "[";
        if (a.length == 0) {
            o << "]";
            return o;
        }
        unsigned int i;
        for (i = 0; i < a.length - 1; i++) {
            o << static_cast<int>(*(a.data + i)) << ", ";
        }
        o << static_cast<int>(*(a.data + i)) << "]";
        return o;
    }

    int8_t operator[] (int index) const;

    double len() const;

    void resize();

    vector_int8 copy() const;

    void append(const int8_t& item);

    int8_t pop(int index);

    vector_int8 clear();

    vector_int8 operator+(const vector_int8& v) const;

    vector_int8 operator+(const int8_t& c) const;

    friend vector_int8 operator+(const int8_t& c, const vector_int8& v) {
        int8x16_t scalar = vdupq_n_s8(c);

        int8_t* dest = new int8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_s8(dest + counter, vaddq_s8(vld1q_s8(v.data + counter), scalar));
        }

        return vector_int8(v.length, dest);
    };

    vector_int8 operator+=(const int8_t& c);

    vector_int8 operator+=(const vector_int8& v);

    vector_int8 operator-(const vector_int8& v) const;

    vector_int8 operator-(const int8_t& c) const;

    friend vector_int8 operator-(const int8_t& c, const vector_int8& v) {
        int8x16_t scalar = vdupq_n_s8(c);

        int8_t* dest = new int8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_s8(dest + counter, vsubq_s8(vld1q_s8(v.data + counter), scalar));
        }

        return vector_int8(v.length, dest);
    }

    vector_int8 operator-=(const int8_t& c);

    vector_int8 operator-=(const vector_int8& v);

    vector_int8 operator-() const;

    vector_int8 operator*(const int8_t& c) const;

    friend vector_int8 operator*(const int8_t& c, const vector_int8& v) {
        int8x16_t scalar = vdupq_n_s8(c);

        int8_t* dest = new int8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_s8(dest + counter, vmulq_s8(vld1q_s8(v.data + counter), scalar));
        }

        return vector_int8(dest, v.size, v.length);
    }

    // No sum here this time
    vector_int8 operator*(const vector_int8& v) const;

    vector_int8 operator*=(const int8_t& c);

    vector_int8 operator*=(const vector_int8& v);

    vector_int8 operator++();

    vector_int8 operator--();

    unsigned int __pivot(const double& limiter = 1e-07) const;

    bool operator==(const int8_t& c) const;

    bool operator==(const vector_int8& v) const;

    bool operator!=(const int8_t& c) const;

    bool operator!=(const vector_int8& v) const;

    bool operator<(const int8_t& c) const;

    bool operator<(const vector_int8& v) const;

    bool operator<=(const int8_t& c) const;

    bool operator<=(const vector_int8& v) const;

    bool operator>(const int8_t& c) const;

    bool operator>(const vector_int8& v) const;

    bool operator>=(const int8_t& c) const;

    bool operator>=(const vector_int8& v) const;

    int32_t dot(const vector_int8& v) const;

    void swap(const unsigned int& first, const unsigned int& second);

    static vector_int8 zero(const unsigned int& dim);

    static vector_int8 one(const unsigned int& dim);

};


#endif //VECTOR_CPP_NEONVECTOR_H
