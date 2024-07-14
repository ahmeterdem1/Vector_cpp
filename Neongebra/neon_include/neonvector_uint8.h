//
// Created by AHMET ERDEM on 7.07.2024.
//

#pragma once

#ifndef VECTOR_CPP_NEONVECTOR_UINT8_H
#define VECTOR_CPP_NEONVECTOR_UINT8_H

#include <ostream>
#include <arm_neon.h>

class vector_uint8 {
public:

    unsigned int size;
    unsigned int length;
    uint8_t* data = nullptr;

    vector_uint8() noexcept;

    vector_uint8(unsigned int data_size, uint8_t* data) noexcept;

    // This special constructor is for internal use only
private:
    vector_uint8(uint8_t* data, unsigned int data_size, unsigned int data_length) noexcept;

public:

    friend std::ostream& operator<< (std::ostream& o, const vector_uint8& a) noexcept {
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

    uint8_t operator[] (int index) const;

    double len() const noexcept;

    void resize() noexcept;

    vector_uint8 copy() const noexcept;

    void append(const uint8_t& item) noexcept;

    uint8_t pop(int index);

    vector_uint8 clear() noexcept;

    vector_uint8 operator+(const vector_uint8& v) const;

    vector_uint8 operator+(const uint8_t& c) const noexcept;

    friend vector_uint8 operator+(const uint8_t& c, const vector_uint8& v) noexcept {
        int8x16_t scalar = vdupq_n_u8(c);

        uint8_t* dest = new uint8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_u8(dest + counter, vaddq_u8(vld1q_u8(v.data + counter), scalar));
        }

        return vector_uint8(v.length, dest);
    };

    vector_uint8 operator+=(const uint8_t& c) noexcept;

    vector_uint8 operator+=(const vector_uint8& v);

    vector_uint8 operator-(const vector_uint8& v) const;

    vector_uint8 operator-(const uint8_t& c) const noexcept;

    friend vector_uint8 operator-(const uint8_t& c, const vector_uint8& v) noexcept {
        int8x16_t scalar = vdupq_n_u8(c);

        uint8_t* dest = new uint8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_u8(dest + counter, vsubq_u8(vld1q_u8(v.data + counter), scalar));
        }

        return vector_uint8(v.length, dest);
    }

    vector_uint8 operator-=(const uint8_t& c) noexcept;

    vector_uint8 operator-=(const vector_uint8& v);

    vector_uint8 operator-() const noexcept;

    vector_uint8 operator*(const uint8_t& c) const noexcept;

    friend vector_uint8 operator*(const uint8_t& c, const vector_uint8& v) noexcept {
        int8x16_t scalar = vdupq_n_u8(c);

        uint8_t* dest = new uint8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_u8(dest + counter, vmulq_u8(vld1q_u8(v.data + counter), scalar));
        }

        return vector_uint8(dest, v.size, v.length);
    }

    // No sum here compared to Vectorgebra implementation
    vector_uint8 operator*(const vector_uint8& v) const;

    vector_uint8 operator*=(const uint8_t& c) noexcept;

    vector_uint8 operator*=(const vector_uint8& v);

    vector_uint8 operator++() noexcept;

    vector_uint8 operator--() noexcept;

private:  // Differently from Vectorgebra, I actually make this private here.
    unsigned int __pivot(const double& limiter = 1e-07) const noexcept;

public:
    bool operator==(const uint8_t& c) const noexcept;

    bool operator==(const vector_uint8& v) const;

    bool operator!=(const uint8_t& c) const noexcept;

    bool operator!=(const vector_uint8& v) const;

    bool operator<(const uint8_t& c) const noexcept;

    bool operator<(const vector_uint8& v) const noexcept;

    bool operator<=(const uint8_t& c) const noexcept;

    bool operator<=(const vector_uint8& v) const noexcept;

    bool operator>(const uint8_t& c) const noexcept;

    bool operator>(const vector_uint8& v) const noexcept;

    bool operator>=(const uint8_t& c) const noexcept;

    bool operator>=(const vector_uint8& v) const noexcept;

    uint32_t dot(const vector_uint8& v) const;

    void swap(const unsigned int& first, const unsigned int& second);

    static vector_uint8 zero(const unsigned int& dim);

    static vector_uint8 one(const unsigned int& dim);

    static vector_uint8 randv(const unsigned int& dim, const uint8_t& low, const uint8_t& high);

};

#endif //VECTOR_CPP_NEONVECTOR_UINT8_H
