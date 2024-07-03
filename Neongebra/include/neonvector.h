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

    int8_t operator[] (int index) const;

    void resize();

    vector_int8 copy() const;

    // Leave .append() to std::move
    void append(const int8_t& item);

    // Leave .pop() to Vectorgebra
    int8_t pop(int index);

    // Same for insert.

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

};

class vector_uint8 {
public:

    unsigned int size;
    unsigned int length;
    uint8_t* data = nullptr;

    vector_uint8();

    vector_uint8(unsigned int data_size, uint8_t* data);

    // This special constructor is for internal use only
private:
    vector_uint8(uint8_t* data, unsigned int data_size, unsigned int data_length);

public:

    friend std::ostream& operator<< (std::ostream& o, const vector_uint8& a) {
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

    uint8_t operator[] (int index) const;

    void resize();

    vector_uint8 copy() const;

    // Leave .append() to std::move
    void append(const uint8_t& item);

    // Leave .pop() to Vectorgebra
    uint8_t pop(int index);

    // Same for insert.

    vector_uint8 clear();

    vector_uint8 operator+(const vector_uint8& v) const;

    vector_uint8 operator+(const uint8_t& c) const;

    friend vector_uint8 operator+(const uint8_t& c, const vector_uint8& v) {
        uint8x16_t scalar = vdupq_n_u8(c);

        uint8_t* dest = new uint8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_u8(dest + counter, vaddq_u8(vld1q_u8(v.data + counter), scalar));
        }

        return vector_uint8(v.length, dest);
    };

    vector_uint8 operator+=(const uint8_t& c);

    vector_uint8 operator+=(const vector_uint8& v);

    vector_uint8 operator-(const vector_uint8& v) const;

    vector_uint8 operator-(const uint8_t& c) const;

    friend vector_uint8 operator-(const uint8_t& c, const vector_uint8& v) {
        uint8x16_t scalar = vdupq_n_u8(c);

        uint8_t* dest = new uint8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_u8(dest + counter, vsubq_u8(vld1q_u8(v.data + counter), scalar));
        }

        return vector_uint8(v.length, dest);
    }

    vector_uint8 operator-=(const uint8_t& c);

    vector_uint8 operator-=(const vector_uint8& v);

    vector_uint8 operator*(const uint8_t& c) const;

    friend vector_uint8 operator*(const uint8_t& c, const vector_uint8& v) {
        uint8x16_t scalar = vdupq_n_u8(c);

        uint8_t* dest = new uint8_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 16) {
            vst1q_u8(dest + counter, vmulq_u8(vld1q_u8(v.data + counter), scalar));
        }

        return vector_uint8(dest, v.size, v.length);
    }

    // No sum here this time
    vector_uint8 operator*(const vector_uint8& v) const;

    vector_uint8 operator*=(const uint8_t& c);

    vector_uint8 operator*=(const vector_uint8& v);

    vector_uint8 operator++();

    vector_uint8 operator--();

};

class vector_int16 {
public:

    unsigned int size;
    unsigned int length;
    int16_t* data = nullptr;

    vector_int16();

    vector_int16(unsigned int data_size, int16_t* data);

    // This special constructor is for internal use only
private:
    vector_int16(int16_t* data, unsigned int data_size, unsigned int data_length);

public:
    void resize();

    vector_int16 copy() const;

    // Leave .append() to std::move
    void append(const int16_t& item);

    // Leave .pop() to Vectorgebra
    int16_t pop(int index);

    // Same for insert.

    vector_int16 clear();

    vector_int16 operator+(const vector_int16& v) const;

    vector_int16 operator+(const int16_t& c) const;

    friend vector_int16 operator+(const int16_t& c, const vector_int16& v) {
        int16x8_t scalar = vdupq_n_s16(c);

        int16_t* dest = new int16_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 8) {
            vst1q_s16(dest + counter, vaddq_s8(vld1q_s16(v.data + counter), scalar));
        }

        return vector_int16(v.length, dest);
    };

    vector_int16 operator+=(const int16_t& c);

    vector_int16 operator+=(const vector_int16& v);

    vector_int16 operator-(const vector_int16& v) const;

    vector_int16 operator-(const int16_t& c) const;

    friend vector_int16 operator-(const int16_t& c, const vector_int16& v) {
        int16x8_t scalar = vdupq_n_s16(c);

        int16_t* dest = new int16_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 8) {
            vst1q_s16(dest + counter, vsubq_s16(vld1q_s16(v.data + counter), scalar));
        }

        return vector_int16(v.length, dest);
    }

    vector_int16 operator-=(const int16_t& c);

    vector_int16 operator-=(const vector_int16& v);

    vector_int16 operator-() const;

    vector_int16 operator*(const int16_t& c) const;

    friend vector_int16 operator*(const int16_t& c, const vector_int16& v) {
        int16x8_t scalar = vdupq_n_s16(c);

        int16_t* dest = new int16_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 8) {
            vst1q_s16(dest + counter, vmulq_s16(vld1q_s16(v.data + counter), scalar));
        }

        return vector_int16(dest, v.size, v.length);
    }

    // No sum here this time
    vector_int16 operator*(const vector_int16& v) const;

    vector_int16 operator*=(const int16_t& c);

    vector_int16 operator*=(const vector_int16& v);

    vector_int16 operator++();

    vector_int16 operator--();

};

class vector_int32 {
public:

    unsigned int size;
    unsigned int length;
    int32_t* data = nullptr;

    vector_int32();

    vector_int32(unsigned int data_size, int32_t* data);

    // This special constructor is for internal use only
private:
    vector_int32(int32_t* data, unsigned int data_size, unsigned int data_length);

public:
    void resize();

    vector_int32 copy() const;

    // Leave .append() to std::move
    void append(const int32_t& item);

    // Leave .pop() to Vectorgebra
    int32_t pop(int index);

    // Same for insert.

    vector_int32 clear();

    vector_int32 operator+(const vector_int32& v) const;

    vector_int32 operator+(const int32_t& c) const;

    friend vector_int32 operator+(const int32_t& c, const vector_int32& v) {
        int32x4_t scalar = vdupq_n_s32(c);


        int32_t* dest = new int32_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 8) {
            vst1q_s32(dest + counter - 4, vaddq_s32(vld1q_s32(v.data + counter - 4), scalar));
            vst1q_s32(dest + counter, vaddq_s32(vld1q_s32(v.data + counter), scalar));
        }

        return vector_int32(v.length, dest);
    };

    vector_int32 operator+=(const int32_t& c);

    vector_int32 operator+=(const vector_int32& v);

    vector_int32 operator-(const vector_int32& v) const;

    vector_int32 operator-(const int32_t& c) const;

    friend vector_int32 operator-(const int32_t& c, const vector_int32& v) {
        int32x4_t scalar = vdupq_n_s32(c);

        int32_t* dest = new int32_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 8) {
            vst1q_s32(dest + counter - 4, vsubq_s32(vld1q_s32(v.data + counter - 4), scalar));
            vst1q_s32(dest + counter, vsubq_s32(vld1q_s32(v.data + counter), scalar));
        }

        return vector_int32(v.length, dest);
    }

    vector_int32 operator-=(const int32_t& c);

    vector_int32 operator-=(const vector_int32& v);

    vector_int32 operator-() const;

    vector_int32 operator*(const int32_t& c) const;

    friend vector_int32 operator*(const int32_t& c, const vector_int32& v) {
        int32x4_t scalar = vdupq_n_s32(c);

        int32_t* dest = new int32_t[v.size];

        for (unsigned int counter = 0; counter < v.size; counter += 8) {
            vst1q_s32(dest + counter - 4, vmulq_s32(vld1q_s32(v.data + counter - 4), scalar));
            vst1q_s32(dest + counter, vmulq_s32(vld1q_s32(v.data + counter), scalar));
        }

        return vector_int32(dest, v.size, v.length);
    }

    // No sum here this time
    vector_int32 operator*(const vector_int32& v) const;

    vector_int32 operator*=(const int32_t& c);

    vector_int32 operator*=(const vector_int32& v);

    vector_int32 operator++();

    vector_int32 operator--();

};

#endif //VECTOR_CPP_NEONVECTOR_H
