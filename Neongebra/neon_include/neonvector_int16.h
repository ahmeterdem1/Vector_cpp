//
// Created by AHMET ERDEM on 15.07.2024.
//

#pragma once

#ifndef VECTOR_CPP_NEONVECTOR_INT16_H
#define VECTOR_CPP_NEONVECTOR_INT16_H

#include <ostream>
#include <arm_neon.h>

class vector_int16 {
public:

    unsigned int size;
    unsigned int length;
    int16_t* data = nullptr;

    vector_int16() noexcept;

    vector_int16(unsigned int data_size, int16_t* data) noexcept;

    // This special constructor is for internal use only
private:
    vector_int16(int16_t* data, unsigned int data_size, unsigned int data_length) noexcept;

public:

    friend std::ostream& operator<< (std::ostream& o, const vector_int16& a) noexcept {
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

    int16_t operator[] (int index) const;

    double len() const noexcept;

    void resize() noexcept;

    vector_int16 copy() const noexcept;

    void append(const int16_t& item) noexcept;

    int16_t pop(int index);

    vector_int16 clear() noexcept;

    vector_int16 operator+(const vector_int16& v) const;

    vector_int16 operator+(const int16_t& c) const noexcept;

    friend vector_int16 operator+(const int16_t& c, const vector_int16& v) noexcept {
        int16x8_t scalar = vdupq_n_s16(c);
        int16x8x2_t temp1;

        int16_t* dest = new int16_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            temp1 = vld2q_s16(v.data + counter);
            *(temp1.val) = vaddq_s16(*(temp1.val), scalar);
            vst2q_s16(dest + counter, temp1);
        }

        return vector_int16(v.length, dest);
    };

    vector_int16 operator+=(const int16_t& c) noexcept;

    vector_int16 operator+=(const vector_int16& v);

    vector_int16 operator-(const vector_int16& v) const;

    vector_int16 operator-(const int16_t& c) const noexcept;

    friend vector_int16 operator-(const int16_t& c, const vector_int16& v) noexcept {
        int16x8_t scalar = vdupq_n_s16(c);
        int16x8x2_t temp1;

        int16_t* dest = new int16_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            temp1 = vld2q_s16(v.data + counter);
            *(temp1.val) = vsubq_s16(*(temp1.val), scalar);
            vst2q_s16(dest + counter, temp1);
        }

        return vector_int16(v.length, dest);
    }

    vector_int16 operator-=(const int16_t& c) noexcept;

    vector_int16 operator-=(const vector_int16& v);

    vector_int16 operator-() const noexcept;

    vector_int16 operator*(const int16_t& c) const noexcept;

    friend vector_int16 operator*(const int16_t& c, const vector_int16& v) noexcept {
        int16x8_t scalar = vdupq_n_s16(c);
        int16x8x2_t temp1;

        int16_t* dest = new int16_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            temp1 = vld2q_s16(v.data + counter);
            *(temp1.val) = vmulq_s16(*(temp1.val), scalar);
            vst2q_s16(dest + counter, temp1);
        }

        return vector_int16(dest, v.size, v.length);
    }

    // No sum here compared to Vectorgebra implementation
    vector_int16 operator*(const vector_int16& v) const;

    vector_int16 operator*=(const int16_t& c) noexcept;

    vector_int16 operator*=(const vector_int16& v);

    vector_int16 operator++() noexcept;

    vector_int16 operator--() noexcept;

private:  // Differently from Vectorgebra, I actually make this private here.
    unsigned int __pivot(const double& limiter = 1e-07) const noexcept;

public:
    bool operator==(const int16_t& c) const noexcept;

    bool operator==(const vector_int16& v) const;

    bool operator!=(const int16_t& c) const noexcept;

    bool operator!=(const vector_int16& v) const;

    bool operator<(const int16_t& c) const noexcept;

    bool operator<(const vector_int16& v) const noexcept;

    bool operator<=(const int16_t& c) const noexcept;

    bool operator<=(const vector_int16& v) const noexcept;

    bool operator>(const int16_t& c) const noexcept;

    bool operator>(const vector_int16& v) const noexcept;

    bool operator>=(const int16_t& c) const noexcept;

    bool operator>=(const vector_int16& v) const noexcept;

    int64_t dot(const vector_int16& v) const;

    void swap(const unsigned int& first, const unsigned int& second);

    static vector_int16 zero(const unsigned int& dim);

    static vector_int16 one(const unsigned int& dim);

    static vector_int16 randv(const unsigned int& dim, const int16_t& low, const int16_t& high);

};

#endif //VECTOR_CPP_NEONVECTOR_INT16_H
