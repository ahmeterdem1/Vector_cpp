//
// Created by AHMET ERDEM on 2.07.2024.
//

#include "../include/neonvector.h"


vector_int8::vector_int8() {
    this->size = 64;
    this->length = 0;
    this->data = new int8_t[this->size];  // Do not put 64 in there, no need to instantiate another int in runtime.
}

vector_int8::vector_int8(unsigned int data_size, int8_t *data) {
    /*
     * In the original Vectorgebra implementation, for the data, just the pointer
     * is copied to this->data. Here, it is changed. A hard copy is done with Q
     * registers. A Q register can hold 16 signed 8-bit integer. SIMD loop is continued
     * until the last 16 or less items, then algorithm switches back to single instruction
     * single data.
     * */
    this->length = data_size;
    this->size = ((this->length % 64) + 1) * 64;
    this->data = new int8_t[this->size];

    unsigned int counter = 0;
    for (; counter < this->size - 16; counter += 16) {
        vst1q_s8(this->data + counter, vld1q_s8(data + counter));
    }

    for (;counter < this->length; counter++) {
        *(this->data + counter) = data[counter];
    }
}

vector_int8::vector_int8(int8_t *data, unsigned int data_size, unsigned int data_length) {
    this->size = data_size;
    this->length = data_length;
    this->data = data;
}

int8_t vector_int8::operator[] (int index) const {
    if (this->length == 0) throw IndexError();
    if (index < 0) index += this->length;
    if (index < 0 or index >= this->length) throw IndexError();
    return *(this->data + index);
}

void vector_int8::resize() {
    unsigned int diff = this->size - this->length;
    diff = (diff % 64) * 64;  // Decrease the size by this amount.

    if (!diff) return;  // Prevent unnecessary hard copies of self.

    this->size -= diff;

    int8_t* new_data = new int8_t[this->size];
    diff = 0;  // Reuse the variable as the loop counter;

    for(; diff < this->size - 16; diff +=16) {
        vst1q_s8(new_data + diff, vld1q_s8(this->data + diff));
    }

    for (;diff < this->length; diff++) {
        *(new_data + diff) = this->data[diff];
    }

    delete[] this->data;
    this->data = new_data;
}

vector_int8 vector_int8::copy() const {
    return vector_int8(this->length, this->data);  // Constructor already does a hard copy
}

void vector_int8::append(const int8_t& item) {

    if (this->length < this->size) {
        this->data[this->length] = item;
        this->length++;
        return;
    }

    int8_t *new_data = new int8_t[this->size + 64];
    int8_t *temp = this->data;
    std::move(temp, temp + this->size, new_data);
    *(new_data + this->size) = item;
    temp = nullptr;
    this->data = new_data;
    this->size += 64;
    this->length += 1;
}

int8_t vector_int8::pop(int index) {
    if (this->length == 0) throw IndexError();
    if (index < 0) index += this->length;
    if (index < 0 or index >= this->length) throw IndexError();

    int8_t r_value = *(this->data + index);

    int i = index;
    for (; i < this->length - 17; i += 16) {
        vst1q_s8(this->data + i, vld1q_s8(this->data + i + 1));
    }

    for (; i < this->length - 1; i++) {
        *(this->data + i) = *(this->data + i + 1);
    }
    this->length--;
    return r_value;
}

vector_int8 vector_int8::clear() {
    delete[] this->data;
    this->size = 64;
    this->data = new int8_t[this->size];
    this->length = 0;
    return *this;
}

vector_int8 vector_int8::operator+(const vector_int8& v) const {
    if (this->length != v.length) throw DimensionError();

    int8_t* dest = new int8_t[this->size];

    // size is guaranteed to be multiple of 64.
    // with length % 64 > 4, below implementation becomes faster
    // than stopping at size - 16, and continuing one by one.
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(dest + counter, vaddq_s8(vld1q_s8(this->data + counter), vld1q_s8(v.data + counter)));
    }

    return vector_int8(dest, this->size, this->length);
}

vector_int8 vector_int8::operator+(const int8_t& c) const {
    int8x16_t scalar = vdupq_n_s8(c);

    int8_t* dest = new int8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(dest + counter, vaddq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return vector_int8(dest, this->size, this->length);
}

vector_int8 vector_int8::operator+=(const int8_t& c) {
    int8x16_t scalar = vdupq_n_s8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vaddq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return *this;
}

vector_int8 vector_int8::operator+=(const vector_int8& v) {
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vaddq_s8(vld1q_s8(this->data + counter), vld1q_s8(v.data + counter)));
    }

    return *this;
}


vector_int8 vector_int8::operator-(const vector_int8& v) const {
    if (this->length != v.length) throw DimensionError();

    int8_t* dest = new int8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(dest + counter, vsubq_s8(vld1q_s8(this->data + counter), vld1q_s8(v.data + counter)));
    }

    return vector_int8(dest, this->size, this->length);
}

vector_int8 vector_int8::operator-(const int8_t& c) const {
    int8x16_t scalar = vdupq_n_s8(c);

    int8_t* dest = new int8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(dest + counter, vsubq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return vector_int8(dest, this->size, this->length);
}

vector_int8 vector_int8::operator-=(const int8_t& c) {
    int8x16_t scalar = vdupq_n_s8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vsubq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return *this;
}

vector_int8 vector_int8::operator-=(const vector_int8& v) {
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vsubq_s8(vld1q_s8(this->data + counter), vld1q_s8(v.data + counter)));
    }

    return *this;
}

vector_int8 vector_int8::operator-() const {
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vnegq_s8(vld1q_s8(this->data + counter)));
    }
    return *this;
}

vector_int8 vector_int8::operator*(const int8_t& c) const {
    int8x16_t scalar = vdupq_n_s8(c);

    int8_t* dest = new int8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(dest + counter, vmulq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return vector_int8(dest, this->size, this->length);
}

vector_int8 vector_int8::operator*(const vector_int8& v) const {
    if (this->length != v.length) throw DimensionError();

    int8_t* dest = new int8_t[this->size];
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(dest + counter, vmulq_s8(vld1q_s8(this->data + counter), vld1q_s8(v.data + counter)));
    }

    return vector_int8(dest, this->size, this->length);
}

vector_int8 vector_int8::operator*=(const int8_t& c) {
    int8x16_t scalar = vdupq_n_s8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vmulq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return *this;
}

vector_int8 vector_int8::operator*=(const vector_int8& v) {
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vmulq_s8(vld1q_s8(this->data + counter), vld1q_s8(v.data + counter)));
    }

    return *this;
}

vector_int8 vector_int8::operator++() {
    int8x16_t scalar = vdupq_n_s8(1);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vmulq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return *this;
}

vector_int8 vector_int8::operator--() {
    int8x16_t scalar = vdupq_n_s8(1);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_s8(this->data + counter, vsubq_s8(vld1q_s8(this->data + counter), scalar));
    }

    return *this;
}











vector_uint8::vector_uint8() {
    this->size = 64;
    this->length = 0;
    this->data = new uint8_t[this->size];  // Do not put 64 in there, no need to instantiate another int in runtime.
}

vector_uint8::vector_uint8(unsigned int data_size, uint8_t *data) {
    /*
     * In the original Vectorgebra implementation, for the data, just the pointer
     * is copied to this->data. Here, it is changed. A hard copy is done with Q
     * registers. A Q register can hold 16 signed 8-bit integer. SIMD loop is continued
     * until the last 16 or less items, then algorithm switches back to single instruction
     * single data.
     * */
    this->length = data_size;
    this->size = ((this->length % 64) + 1) * 64;
    this->data = new uint8_t[this->size];

    unsigned int counter = 0;
    for (; counter < this->size - 16; counter += 16) {
        vst1q_u8(this->data + counter, vld1q_u8(data + counter));
    }

    for (;counter < this->length; counter++) {
        *(this->data + counter) = data[counter];
    }
}

vector_uint8::vector_uint8(uint8_t *data, unsigned int data_size, unsigned int data_length) {
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

void vector_uint8::resize() {
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

vector_uint8 vector_uint8::copy() const {
    return vector_uint8(this->length, this->data);  // Constructor already does a hard copy
}

void vector_uint8::append(const uint8_t& item) {

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

    uint8_t r_value = *(this->data + index);

    int i = index;
    for (; i < this->length - 17; i += 16) {
        vst1q_u8(this->data + i, vld1q_u8(this->data + i + 1));
    }

    for (; i < this->length - 1; i++) {
        *(this->data + i) = *(this->data + i + 1);
    }
    this->length--;
    return r_value;
}

vector_uint8 vector_uint8::clear() {
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

vector_uint8 vector_uint8::operator+(const uint8_t& c) const {
    uint8x16_t scalar = vdupq_n_u8(c);

    uint8_t* dest = new uint8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vaddq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator+=(const uint8_t& c) {
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

vector_uint8 vector_uint8::operator-(const uint8_t& c) const {
    uint8x16_t scalar = vdupq_n_u8(c);

    uint8_t* dest = new uint8_t[this->size];

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(dest + counter, vsubq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return vector_uint8(dest, this->size, this->length);
}

vector_uint8 vector_uint8::operator-=(const uint8_t& c) {
    uint8x16_t scalar = vdupq_n_u8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vsubq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

vector_uint8 vector_uint8::operator-=(const vector_uint8& v) {
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vsubq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return *this;
}

vector_uint8 vector_uint8::operator*(const uint8_t& c) const {
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

vector_uint8 vector_uint8::operator*=(const uint8_t& c) {
    uint8x16_t scalar = vdupq_n_u8(c);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vmulq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

vector_uint8 vector_uint8::operator*=(const vector_uint8& v) {
    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vmulq_u8(vld1q_u8(this->data + counter), vld1q_u8(v.data + counter)));
    }

    return *this;
}

vector_uint8 vector_uint8::operator++() {
    uint8x16_t scalar = vdupq_n_u8(1);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vmulq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}

vector_uint8 vector_uint8::operator--() {
    uint8x16_t scalar = vdupq_n_u8(1);

    for (unsigned int counter = 0; counter < this->size; counter += 16) {
        vst1q_u8(this->data + counter, vsubq_u8(vld1q_u8(this->data + counter), scalar));
    }

    return *this;
}



