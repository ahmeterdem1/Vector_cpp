//
// Created by AHMET ERDEM on 2.07.2024.
//

#pragma once

#ifndef VECTOR_CPP_NEONVECTOR_H
#define VECTOR_CPP_NEONVECTOR_H

#include <arm_neon.h>
#include <ostream>
// Important, this->size is NEVER zero for neon supported vectors.
// And, they are ALWAYS a multiple of 64.
// This is for consistency with SIMD operations.

// Another rule of thumb, counter variable of every loop
// must be unsigned integer, instead of integer.

class vector_int8 {
public:

    unsigned int size;
    unsigned int length;
    int8_t* data = nullptr;

    vector_int8() noexcept;

    vector_int8(unsigned int data_size, int8_t* data) noexcept;

    // This special constructor is for internal use only
private:
    vector_int8(int8_t* data, unsigned int data_size, unsigned int data_length) noexcept;

public:

    friend std::ostream& operator<< (std::ostream& o, const vector_int8& a) noexcept {
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

    double len() const noexcept;

    void resize() noexcept;

    vector_int8 copy() const noexcept;

    void append(const int8_t& item) noexcept;

    int8_t pop(int index);

    vector_int8 clear() noexcept;

    vector_int8 operator+(const vector_int8& v) const;

    vector_int8 operator+(const int8_t& c) const noexcept;

    friend vector_int8 operator+(const int8_t& c, const vector_int8& v) noexcept {
        int8x16_t scalar = vdupq_n_s8(c);

        int8_t* dest = new int8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_s8(dest + counter, vaddq_s8(vld1q_s8(v.data + counter), scalar));
        }

        return vector_int8(v.length, dest);
    };

    vector_int8 operator+=(const int8_t& c) noexcept;

    vector_int8 operator+=(const vector_int8& v);

    vector_int8 operator-(const vector_int8& v) const;

    vector_int8 operator-(const int8_t& c) const noexcept;

    friend vector_int8 operator-(const int8_t& c, const vector_int8& v) noexcept {
        int8x16_t scalar = vdupq_n_s8(c);

        int8_t* dest = new int8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_s8(dest + counter, vsubq_s8(vld1q_s8(v.data + counter), scalar));
        }

        return vector_int8(v.length, dest);
    }

    vector_int8 operator-=(const int8_t& c) noexcept;

    vector_int8 operator-=(const vector_int8& v);

    vector_int8 operator-() const noexcept;

    vector_int8 operator*(const int8_t& c) const noexcept;

    friend vector_int8 operator*(const int8_t& c, const vector_int8& v) noexcept {
        int8x16_t scalar = vdupq_n_s8(c);

        int8_t* dest = new int8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_s8(dest + counter, vmulq_s8(vld1q_s8(v.data + counter), scalar));
        }

        return vector_int8(dest, v.size, v.length);
    }

    // No sum here compared to Vectorgebra implementation
    vector_int8 operator*(const vector_int8& v) const;

    vector_int8 operator*=(const int8_t& c) noexcept;

    vector_int8 operator*=(const vector_int8& v);

    vector_int8 operator++() noexcept;

    vector_int8 operator--() noexcept;

private:  // Differently from Vectorgebra, I actually make this private here.
    unsigned int __pivot(const double& limiter = 1e-07) const noexcept;

public:
    bool operator==(const int8_t& c) const noexcept;

    bool operator==(const vector_int8& v) const;

    bool operator!=(const int8_t& c) const noexcept;

    bool operator!=(const vector_int8& v) const;

    bool operator<(const int8_t& c) const noexcept;

    bool operator<(const vector_int8& v) const noexcept;

    bool operator<=(const int8_t& c) const noexcept;

    bool operator<=(const vector_int8& v) const noexcept;

    bool operator>(const int8_t& c) const noexcept;

    bool operator>(const vector_int8& v) const noexcept;

    bool operator>=(const int8_t& c) const noexcept;

    bool operator>=(const vector_int8& v) const noexcept;

    int32_t dot(const vector_int8& v) const;

    void swap(const unsigned int& first, const unsigned int& second);

    static vector_int8 zero(const unsigned int& dim);

    static vector_int8 one(const unsigned int& dim);

    static vector_int8 randv(const unsigned int& dim, const int8_t& low, const int8_t& high);

};


#endif //VECTOR_CPP_NEONVECTOR_H
