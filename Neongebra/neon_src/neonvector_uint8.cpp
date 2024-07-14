//
// Created by AHMET ERDEM on 7.07.2024.
//

#include "../neon_include/neonvector_uint8.h"
#include "../include/functions.h"
#include <algorithm>

vector_uint8::vector_uint8() noexcept {
    this->size = 64;
    this->length = 0;
    this->data = new uint8_t[this->size];  // Do not put 64 in there, no need to instantiate another int in runtime.
}

vector_uint8::vector_uint8(unsigned int data_size, uint8_t *data) noexcept {
    /*
     * In the original Vectorgebra implementation, for the data, just the pointer
     * is copied to this->data. Here, it is changed. A hard copy is done with Q
     * registers. A Q register can hold 16 signed 8-bit integer. SIMD loop is continued
     * until the last 16 or less items, then algorithm switches back to single instruction
     * single data.
     * */
    this->length = data_size;
    this->size = (floor(this->length / 64) + 1) * 64;
    this->data = new uint8_t[this->size];

    unsigned int counter = 0;
    for (; counter < this->size - 16; counter += 16) {
        vst1q_u8(this->data + counter, vld1q_u8(data + counter));
    }

    for (;counter < this->length; counter++) {
        *(this->data + counter) = data[counter];
    }
}

vector_uint8::vector_uint8(uint8_t *data, unsigned int data_size, unsigned int data_length) noexcept {
    this->size = data_size;
    this->length = data_length;
    this->data = data;
}

uint8_t vector_uint8::operator[] (int index) const {
    if (this->length == 0) throw IndexError();
    if (index < 0) index += this->length;
    if (index < 0 or index >= this->length) throw IndexError();
    return *(this->data + index);
}

double vector_uint8::len() const noexcept {
    unsigned int counter = 0;

    uint16x8_t temp_holder;  // This is to prevent multiple load operations in each loop

    uint64x2_t sum64 = vdupq_n_u64(0);

    uint32_t sum_holder;
    uint32_t sum = 0;


    for(; counter < this->length - this->length % 16; counter += 16) {
        temp_holder = vld1q_u8(this->data + counter);
        sum64 = vaddq_u64(vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(vmulq_u8(temp_holder, temp_holder)))), sum64);
    }

    vst1_lane_u32(&sum_holder, vreinterpret_u32_u64(vadd_u64(vget_low_u64(sum64), vget_high_u64(sum64))), 0);
    sum += sum_holder;

    for(; counter < this->length; counter++) {
        sum += *(this->data + counter) * *(this->data + counter);
    }

    return std::sqrt(sum);  // std:: there to differentiate from the Vectorgebra sqrt
}

void vector_uint8::resize() noexcept {
    unsigned int diff = this->size - this->length;
    diff = (diff % 64) * 64;  // Decrease the size by this amount.

    if (!diff) return;  // Prevent unnecessary hard copies of self.

    this->size -= diff;

    uint8_t* new_data = new uint8_t[this->size];
    diff = 0;  // Reuse the variable as the loop counter;

    for(; diff < this->size - 16; diff +=16) {
        vst1q_u8(new_data + diff, vld1q_u8(this->data + diff));
    }

    for (;diff < this->length; diff++) {
        *(new_data + diff) = this->data[diff];
    }

    delete[] this->data;
    this->data = new_data;
}

vector_uint8 vector_uint8::copy() const noexcept {
    return vector_uint8(this->length, this->data);  // Constructor already does a hard copy
}

void vector_uint8::append(const uint8_t& item) noexcept {

    if (this->length < this->size) {
        this->data[this->length] = item;
        this->length++;
        return;
    }

    uint8_t *new_data = new uint8_t[this->size + 64];
    uint8_t *temp = this->data;
    std::move(temp, temp + this->size, new_data);
    *(new_data + this->size) = item;
    temp = nullptr;
    this->data = new_data;
    this->size += 64;
    this->length += 1;
}

uint8_t vector_uint8::pop(int index) {
    if (this->length == 0) throw IndexError();
    if (index < 0) index += this->length;
    if (index < 0 or index >= this->length) throw IndexError();

    int8_t r_value = *(this->data + index);

    unsigned int i = index;
    for (; i < this->length - 17; i += 16) {
        vst1q_u8(this->data + i, vld1q_u8(this->data + i + 1));
    }

    for (; i < this->length - 1; i++) {
        *(this->data + i) = *(this->data + i + 1);
    }
    this->length--;
    return r_value;
}

vector_uint8 vector_uint8::clear() noexcept {
    delete[] this->data;
    this->size = 64;
    this->data = new uint8_t[this->size];
    this->length = 0;
    return *this;
}

vector_uint8 vector_uint8::operator+(const vector_uint8& v) const {
    if (this->length != v.length) throw DimensionError();

    uint8_t* dest = new uint8_t[this->size];

    // size is guaranteed to be multiple of 64.
    // with length % 64 > 4, below implementation becomes faster
    // than stopping at size - 16, and continuing one by one.
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vaddq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator+(const uint8_t& c) const noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);

    uint8_t* dest = new uint8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vaddq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator+=(const uint8_t& c) noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vaddq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

vector_uint8 vector_uint8::operator+=(const vector_uint8& v) {
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vaddq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return *this;
}

vector_uint8 vector_uint8::operator-(const vector_uint8& v) const {
    if (this->length != v.length) throw DimensionError();

    uint8_t* dest = new uint8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vsubq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator-(const uint8_t& c) const noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);

    uint8_t* dest = new uint8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vsubq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator-=(const uint8_t& c) noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vsubq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

vector_uint8 vector_uint8::operator-=(const vector_uint8& v) {
    if (this->length != v.length) throw DimensionError();

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vsubq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return *this;
}

vector_uint8 vector_uint8::operator-() const noexcept {
    uint8x16_t scalar = vdupq_n_u8(255);
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        // This is, multiplying by -1 for unsigned integers.
        vst1q_u8(this->data + counter, vaddq_u8(vld1q_u8(this->data + counter), scalar));
    }
    return *this;
}

vector_uint8 vector_uint8::operator*(const uint8_t& c) const noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);

    uint8_t* dest = new uint8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vmulq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator*(const vector_uint8& v) const {
    if (this->length != v.length) throw DimensionError();

    uint8_t* dest = new uint8_t[this->size];
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vmulq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator*=(const uint8_t& c) noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vmulq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

vector_uint8 vector_uint8::operator*=(const vector_uint8& v) {
    if (this->length != v.length) throw DimensionError();

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vmulq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return *this;
}

vector_uint8 vector_uint8::operator++() noexcept {
    uint8x16_t scalar = vdupq_n_u8(1);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vmulq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

vector_uint8 vector_uint8::operator--() noexcept {
    uint8x16_t scalar = vdupq_n_u8(1);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vsubq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

unsigned int vector_uint8::__pivot(const double& limiter) const noexcept {
    // This method, may be strange for unsigned values.
    if (this->length == 0) return 0;
    for (unsigned int i = 0; i < this->length; i++) {
        if (abs(*(this->data + i)) > limiter) return i;
    }
    return this->length;
}

bool vector_uint8::operator==(const uint8_t& c) const noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);
    uint8x16_t control = vdupq_n_u8(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 16; counter += 16) {
        if (vminvq_u8(veorq_u8(vceqq_u8(vld1q_u8(this->data + counter), scalar), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) != c) return false;
    }

    return true;
}

bool vector_uint8::operator==(const vector_uint8& v) const {
    if (this->length != v.length) throw DimensionError();

    uint8x16_t control = vdupq_n_u8(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 16; counter += 16) {
        if (vminvq_u8(veorq_u8(vceqq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) != *(v.data + counter)) return false;
    }

    return true;
}

bool vector_uint8::operator!=(const uint8_t& c) const noexcept {
    uint8x16_t scalar = vdupq_n_u8(c);
    uint8x16_t control = vdupq_n_u8(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 16; counter += 16) {
        if (vminvq_u8(veorq_u8(vmvnq_u8(vceqq_u8(vld1q_u8(this->data + counter), scalar)), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) == c) return false;
    }

    return true;
}

bool vector_uint8::operator!=(const vector_uint8& v) const {
    if (this->length != v.length) throw DimensionError();

    uint8x16_t control = vdupq_n_u8(0xFF);

    unsigned int counter = 0;

    for(; counter < this->length - this->length % 16; counter += 16) {
        if (vminvq_u8(veorq_u8(vmvnq_u8(vceqq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter))), control)) != 0)
            return false;
    }

    for (; counter < this->length; counter++) {
        if (*(this->data + counter) == *(v.data + counter)) return false;
    }

    return true;
}

bool vector_uint8::operator<(const uint8_t& c) const noexcept {
    return (this->len() < c);
}

bool vector_uint8::operator<(const vector_uint8& v) const noexcept {
    return (this->len() < v.len());
}

bool vector_uint8::operator<=(const uint8_t& c) const noexcept {
    return (this->len() <= c);
}

bool vector_uint8::operator<=(const vector_uint8& v) const noexcept {
    return (this->len() <= v.len());
}

bool vector_uint8::operator>(const uint8_t& c) const noexcept {
    return (this->len() > c);
}

bool vector_uint8::operator>(const vector_uint8& v) const noexcept {
    return (this->len() > v.len());
}

bool vector_uint8::operator>=(const uint8_t& c) const noexcept {
    return (this->len() >= c);
}

bool vector_uint8::operator>=(const vector_uint8& v) const noexcept {
    return (this->len() >= v.len());
}

uint32_t vector_uint8::dot(const vector_uint8& v) const {
    if (this->length != v.length) throw DimensionError();

    unsigned int counter = 0;

    uint64x2_t sum64 = vdupq_n_u64(0);
    uint32_t sum_holder;
    uint32_t sum = 0;


    // Normally, each loop here corresponds to 15 * and 16 + instructions, except loadings.
    // This implementation in the main loop, in total applies 10 instructions, except loadings.
    // Theoretically, this needs to be ~3 times faster than the SISD approach.

    for(; counter < this->length - this->length % 16; counter += 16) {
        sum64 = vaddq_u64(vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(vmulq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter))))),
                          sum64);
    }

    vst1_lane_u32(&sum_holder, vreinterpret_u32_u64(vadd_u64(vget_low_u64(sum64), vget_high_u64(sum64))), 0);
    sum += sum_holder;

    for(; counter < this->length; counter++) {
        sum += *(this->data + counter) * *(v.data + counter);
    }

    return sum;
}

void vector_uint8::swap(const unsigned int &first, const unsigned int &second) {
    if (first >= this->length or first < 0 or second >= this->length or second < 0) throw RangeError();

    uint8_t temp = *(this->data + first);
    *(this->data + first) = *(this->data + second);
    *(this->data + second) = temp;
}

vector_uint8 vector_uint8::zero(const unsigned int& dim) {
    if (dim < 0) throw RangeError();
    const unsigned int size = 64 * (floor(dim / 64) + 1);
    uint8_t *temp = new uint8_t[size];
    for (unsigned int i = 0; i < dim; i++) {
        *(temp + i) = 0;
    }
    return vector_uint8(temp, size, dim);
}

vector_uint8 vector_uint8::one(const unsigned int& dim) {
    if (dim < 0) throw RangeError();
    const unsigned int size = 64 * (floor(dim / 64) + 1);
    uint8_t *temp = new uint8_t[size];
    for (unsigned int i = 0; i < dim; i++) {
        *(temp + i) = 1;
    }
    return vector_uint8(temp, size, dim);
}

vector_uint8 vector_uint8::randv(const unsigned int &dim, const uint8_t &low, const uint8_t &high) {
    if (dim < 0 or low >= high) throw RangeError();
    const unsigned int size = 64 * (floor(dim / 64) + 1);
    uint8_t *temp = new uint8_t[size];
    for (unsigned int i = 0; i < dim; i++) {
        *(temp + i) = randint(low, high);
    }
    return vector_uint8(temp, size, dim);
}


