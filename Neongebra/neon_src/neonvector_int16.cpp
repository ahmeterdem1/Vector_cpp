//
// Created by AHMET ERDEM on 15.07.2024.
//

#include "../neon_include/neonvector_int16.h"
#include "../include/functions.h"
#include <algorithm>


vector_int16::vector_int16() noexcept {
    this->size = 64;
    this->length = 0;
    this->data = new int16_t[this->size];  // Do not put 64 in there, no need to instantiate another int in runtime.
}

vector_int16::vector_int16(unsigned int data_size, int16_t *data) noexcept {
/*
 * In the original Vectorgebra implementation, for the data, just the pointer
 * is copied to this->data. Here, it is changed. A hard copy is done with Q
 * registers. A Q register can hold 16 signed 8-bit integer. SIMD loop is continued
 * until the last 16 or less items, then algorithm switches back to single instruction
 * single data.
 * */
    this->length = data_size;
    this->size = (floor(this->length / 64) + 1) * 64;
    this->data = new int16_t[this->size];

    unsigned int counter = 0;

    // Since size is always a multiple of 64, and probably different from the length,
    // we stop at last N, where N is the data size of the vector.
    for (; counter < this->size - 8; counter += 8) {
        vst1q_s16(this->data + counter, vld1q_s16(data + counter));
    }

    for (;counter < this->length; counter++) {
        *(this->data + counter) = data[counter];
    }
}

vector_int16::vector_int16(int16_t *data, unsigned int data_size, unsigned int data_length) noexcept {
    this->size = data_size;
    this->length = data_length;
    this->data = data;
}

int16_t vector_int16::operator[] (int index) const {
    if (this->length == 0) throw IndexError();
    if (index < 0) index += this->length;
    if (index < 0 or index >= this->length) throw IndexError();
    return *(this->data + index);
}

double vector_int16::len() const noexcept {
    unsigned int counter = 0;

    int16x8x2_t temp1;

    int64x2_t sum64 = vdupq_n_s64(0);

    int64_t sum_holder;
    int64_t sum = 0;


    for(; counter < this->length - this->length % 16; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        sum64 = vaddq_s64(vpaddlq_s32(vpaddlq_s16(vmulq_s16(*(temp1.val), *(temp1.val)))), sum64);
        sum64 = vaddq_s64(vpaddlq_s32(vpaddlq_s16(vmulq_s16(*(temp1.val + 1), *(temp1.val + 1)))), sum64);
    }

    vst1_lane_s64(&sum_holder, vadd_s64(vget_low_s64(sum64), vget_high_s64(sum64)), 0);
    sum += sum_holder;

    for(; counter < this->length; counter++) {
        sum += *(this->data + counter) * *(this->data + counter);
    }

    return std::sqrt(sum);  // std:: there to differentiate from the Vectorgebra sqrt
}

void vector_int16::resize() noexcept {
    unsigned int diff = this->size - this->length;
    diff = (diff % 64) * 64;  // Decrease the size by this amount.

    if (!diff) return;  // Prevent unnecessary hard copies of self.

    this->size -= diff;

    int16_t* new_data = new int16_t[this->size];
    diff = 0;  // Reuse the variable as the loop counter;

    for(; diff < this->size - 8; diff += 8) {
        vst1q_s16(new_data + diff, vld1q_s16(this->data + diff));
    }

    for (;diff < this->length; diff++) {
        *(new_data + diff) = *(this->data + diff);
    }

    delete[] this->data;
    this->data = new_data;
}

vector_int16 vector_int16::copy() const noexcept {
    return vector_int16(this->length, this->data);  // Constructor already does a hard copy
}

void vector_int16::append(const int16_t& item) noexcept {

    if (this->length < this->size) {
    this->data[this->length] = item;
    this->length++;
    return;
    }

    int16_t *new_data = new int16_t[this->size + 64];
    int16_t *temp = this->data;
    std::move(temp, temp + this->size, new_data);
    *(new_data + this->size) = item;
    temp = nullptr;
    this->data = new_data;
    this->size += 64;
    this->length += 1;
}

int16_t vector_int16::pop(int index) {
    if (this->length == 0) throw IndexError();
    if (index < 0) index += this->length;
    if (index < 0 or index >= this->length) throw IndexError();

    int16_t r_value = *(this->data + index);

    unsigned int i = index;
    for (; i < this->length - 9; i += 8) {
        vst1q_s16(this->data + i, vld1q_s16(this->data + i + 1));
    }

    for (; i < this->length - 1; i++) {
        *(this->data + i) = *(this->data + i + 1);
    }
    this->length--;
    return r_value;
}

vector_int16 vector_int16::clear() noexcept {
    delete[] this->data;
    this->size = 64;
    this->data = new int16_t[this->size];
    this->length = 0;
    return *this;
}

vector_int16 vector_int16::operator+(const vector_int16& v) const {
    if (this->length != v.length) throw DimensionError();

    int16_t* dest = new int16_t[this->size];

    int16x8x2_t temp1;
    int16x8x2_t temp2;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        temp2 = vld2q_s16(v.data + counter);

        // Save RAM by reusing temp1
        *(temp1.val) = vaddq_s16(*(temp1.val), *(temp2.val));
        *(temp1.val + 1) = vaddq_s16(*(temp1.val + 1), *(temp2.val + 1));
        vst2q_s16(dest + counter, temp1);

    }

    return vector_int16(dest, this->size, this->length);
}

vector_int16 vector_int16::operator+(const int16_t& c) const noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8x2_t temp1;

    int16_t* dest = new int16_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vaddq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vaddq_s16(*(temp1.val + 1), scalar);
        vst2q_s16(dest + counter, temp1);
    }

    return vector_int16(dest, this->size, this->length);
}

vector_int16 vector_int16::operator+=(const int16_t& c) noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8x2_t temp1;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vaddq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vaddq_s16(*(temp1.val), scalar);
        vst2q_s16(this->data + counter, temp1);

    }

    return *this;
}

vector_int16 vector_int16::operator+=(const vector_int16& v) {
    if (this->length != v.length) throw DimensionError();

    int16x8x2_t temp1;
    int16x8x2_t temp2;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        temp2 = vld2q_s16(v.data + counter);
        *(temp1.val) = vaddq_s16(*(temp1.val), *(temp2.val));
        *(temp1.val + 1) = vaddq_s16(*(temp1.val + 1), *(temp2.val + 1));
        vst2q_s16(this->data + counter, temp1);
    }

    return *this;
}

vector_int16 vector_int16::operator-(const vector_int16& v) const {
    if (this->length != v.length) throw DimensionError();

    int16_t* dest = new int16_t[this->size];
    int16x8x2_t temp1;
    int16x8x2_t temp2;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        temp2 = vld2q_s16(v.data + counter);
        *(temp1.val) = vsubq_s16(*(temp1.val), *(temp2.val));
        *(temp1.val + 1) = vsubq_s16(*(temp1.val + 1), *(temp2.val + 1));
        vst2q_s16(dest + counter, temp1);
    }

    return vector_int16(dest, this->size, this->length);
}

vector_int16 vector_int16::operator-(const int16_t& c) const noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8x2_t temp1;
    int16_t* dest = new int16_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vsubq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vsubq_s16(*(temp1.val + 1), scalar);
        vst2q_s16(dest + counter, temp1);
    }

    return vector_int16(dest, this->size, this->length);
}

vector_int16 vector_int16::operator-=(const int16_t& c) noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8x2_t temp1;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vsubq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vsubq_s16(*(temp1.val + 1), scalar);
        vst2q_s16(this->data + counter, temp1);
    }

    return *this;
}

vector_int16 vector_int16::operator-=(const vector_int16& v) {
    if (this->length != v.length) throw DimensionError();
    int16x8x2_t temp1;
    int16x8x2_t temp2;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        temp2 = vld2q_s16(v.data + counter);
        *(temp1.val) = vsubq_s16(*(temp1.val), *(temp2.val));
        *(temp1.val + 1) = vsubq_s16(*(temp1.val + 1), *(temp2.val + 1));
        vst2q_s16(this->data + counter, temp1);
    }

    return *this;
}

vector_int16 vector_int16::operator-() const noexcept {
    for (unsigned int counter = 0; counter < this->size; counter += 8) {
        vst1q_s16(this->data + counter, vnegq_s16(vld1q_s16(this->data + counter)));
    }
    return *this;
}

vector_int16 vector_int16::operator*(const int16_t& c) const noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8x2_t temp1;

    int16_t* dest = new int16_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vmulq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vmulq_s16(*(temp1.val + 1), scalar);
        vst2q_s16(dest + counter, temp1);
    }

    return vector_int16(dest, this->size, this->length);
}

vector_int16 vector_int16::operator*(const vector_int16& v) const {
    if (this->length != v.length) throw DimensionError();
    int16x8x2_t temp1;
    int16x8x2_t temp2;

    int16_t* dest = new int16_t[this->size];
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        temp2 = vld2q_s16(v.data + counter);
        *(temp1.val) = vmulq_s16(*(temp1.val), *(temp2.val));
        *(temp1.val + 1) = vmulq_s16(*(temp1.val + 1), *(temp2.val + 1));
        vst2q_s16(dest + counter, temp1);
    }

    return vector_int16(dest, this->size, this->length);
}

vector_int16 vector_int16::operator*=(const int16_t& c) noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8x2_t temp1;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vmulq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vmulq_s16(*(temp1.val + 1), scalar);
        vst2q_s16(this->data, temp1);
    }

    return *this;
}

vector_int16 vector_int16::operator*=(const vector_int16& v) {
    if (this->length != v.length) throw DimensionError();
    int16x8x2_t temp1;
    int16x8x2_t temp2;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        temp2 = vld2q_s16(v.data + counter);
        *(temp1.val) = vmulq_s16(*(temp1.val), *(temp2.val));
        *(temp1.val + 1) = vmulq_s16(*(temp1.val + 1), *(temp2.val + 1));
        vst2q_s16(this->data, temp1);
    }

    return *this;
}

vector_int16 vector_int16::operator++() noexcept {
    int16x8_t scalar = vdupq_n_s16(1);
    int16x8x2_t temp1;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vaddq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vaddq_s16(*(temp1.val + 1), scalar);
        vst2q_s16(this->data + counter, temp1);
    }

    return *this;
}

vector_int16 vector_int16::operator--() noexcept {
    int16x8_t scalar = vdupq_n_s16(1);
    int16x8x2_t temp1;

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        *(temp1.val) = vsubq_s16(*(temp1.val), scalar);
        *(temp1.val + 1) = vsubq_s16(*(temp1.val + 1), scalar);
        vst2q_s16(this->data + counter, temp1);
    }

    return *this;
}

unsigned int vector_int16::__pivot(const double& limiter) const noexcept {
    if (this->length == 0) return 0;

    for (unsigned int i = 0; i < this->length; i++) {
        if (abs(*(this->data + i)) > limiter) return i;
    }
    return this->length;
}

bool vector_int16::operator==(const int16_t& c) const noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8_t control = vdupq_n_s16(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 8; counter += 8) {
        if (vminvq_s16(veorq_s16(vceqq_s16(vld1q_s16(this->data + counter), scalar), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) != c) return false;
    }

    return true;
}

bool vector_int16::operator==(const vector_int16& v) const {
    if (this->length != v.length) throw DimensionError();

    int16x8_t control = vdupq_n_s16(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 8; counter += 8) {
        if (vminvq_s16(veorq_s16(vceqq_s16(vld1q_s16(this->data + counter), vld1q_s16(v.data + counter)), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) != *(v.data + counter)) return false;
    }

    return true;
}

bool vector_int16::operator!=(const int16_t& c) const noexcept {
    int16x8_t scalar = vdupq_n_s16(c);
    int16x8_t control = vdupq_n_s16(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 8; counter += 8) {
        if (vminvq_s16(veorq_s16(vmvnq_s16(vceqq_s16(vld1q_s16(this->data + counter), scalar)), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) == c) return false;
    }

    return true;
}

bool vector_int16::operator!=(const vector_int16& v) const {
    if (this->length != v.length) throw DimensionError();

    int16x8_t control = vdupq_n_s16(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 8; counter += 8) {
        if (vminvq_s16(veorq_s16(vmvnq_s16(vceqq_s16(vld1q_s16(this->data + counter), vld1q_s16(v.data + counter))), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) == *(v.data + counter)) return false;
    }

    return true;
}

bool vector_int16::operator<(const int16_t& c) const noexcept {
    return (this->len() < c);
}

bool vector_int16::operator<(const vector_int16& v) const noexcept {
    return (this->len() < v.len());
}

bool vector_int16::operator<=(const int16_t& c) const noexcept {
    return (this->len() <= c);
}

bool vector_int16::operator<=(const vector_int16& v) const noexcept {
    return (this->len() <= v.len());
}

bool vector_int16::operator>(const int16_t& c) const noexcept {
    return (this->len() > c);
}

bool vector_int16::operator>(const vector_int16& v) const noexcept {
    return (this->len() > v.len());
}

bool vector_int16::operator>=(const int16_t& c) const noexcept {
    return (this->len() >= c);
}

bool vector_int16::operator>=(const vector_int16& v) const noexcept {
    return (this->len() >= v.len());
}

int64_t vector_int16::dot(const vector_int16& v) const {
    if (this->length != v.length) throw DimensionError();

    unsigned int counter = 0;
    int16x8x2_t temp1;
    int16x8x2_t temp2;

    int64x2_t sum64 = vdupq_n_s64(0);
    int64_t sum_holder;
    int64_t sum = 0;

    for(; counter < this->length - this->length % 16; counter += 16) {
        temp1 = vld2q_s16(this->data + counter);
        temp2 = vld2q_s16(v.data + counter);
        *(temp1.val) = vmulq_s16(*(temp1.val), *(temp2.val));
        *(temp1.val + 1) = vmulq_s16(*(temp1.val + 1), *(temp2.val + 1));
        sum64 = vaddq_s64(vpaddlq_s32(vpaddlq_s16(*(temp1.val))), sum64);
        sum64 = vaddq_s64(vpaddlq_s32(vpaddlq_s16(*(temp1.val + 1))), sum64);
    }

    vst1_lane_s64(&sum_holder, vadd_s64(vget_low_s64(sum64), vget_high_s64(sum64)), 0);
    sum += sum_holder;

    for(; counter < this->length; counter++) {
        sum += *(this->data + counter) * *(v.data + counter);
    }

    return sum;
}

void vector_int16::swap(const unsigned int &first, const unsigned int &second) {
    if (first >= this->length or first < 0 or second >= this->length or second < 0) throw RangeError();

    int16_t temp = *(this->data + first);
    *(this->data + first) = *(this->data + second);
    *(this->data + second) = temp;
}

vector_int16 vector_int16::zero(const unsigned int& dim) {
    if (dim < 0) throw RangeError();
    const unsigned int size = 64 * (floor(dim / 64) + 1);
    int16_t *temp = new int16_t[size];
    for (unsigned int i = 0; i < dim; i++) {
        *(temp + i) = 0;
    }
    return vector_int16(temp, size, dim);
}

vector_int16 vector_int16::one(const unsigned int& dim) {
    if (dim < 0) throw RangeError();
    const unsigned int size = 64 * (floor(dim / 64) + 1);
    int16_t *temp = new int16_t[size];
    for (unsigned int i = 0; i < dim; i++) {
        *(temp + i) = 1;
    }
    return vector_int16(temp, size, dim);
}

vector_int16 vector_int16::randv(const unsigned int &dim, const int16_t &low, const int16_t &high) {
    if (dim < 0 or low >= high) throw RangeError();
    const unsigned int size = 64 * (floor(dim / 64) + 1);
    int16_t *temp = new int16_t[size];
    for (unsigned int i = 0; i < dim; i++) {
        *(temp + i) = randint(low, high);
    }
    return vector_int16(temp, size, dim);
}


