//
// Created by AHMET ERDEM on 4.09.2023.
//

#include <iostream>
#include <cmath>

#ifndef VECTOR_CPP_VECTOR_H
#define VECTOR_CPP_VECTOR_H

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"

#define unimportant_divisor 3137

// Helper functions

int Random(){
    // For this function to be useful, srand() must be
    // seeded in the main with time(0) or something.
    // This is only for limited use, obviously.

    srand(rand());
    int next_random = rand();
    return next_random;
}

std::string* split(std::string in, char limiter){
    unsigned long len = in.length();
    int delimiter = 0;
    int amount = 0;

    for (int i = 0; i < len; i++){
        if (in[i] == limiter){
            amount += 1;
        }
    }

    std::string *res = new std::string[amount + 1];
    int counter = 0;

    for (int i = 0; i < len; i++){
        if (in[i] == limiter){
            std::string temp = "";
            for (int j = delimiter; j < i; j++){
                temp += in[j];
            }
            res[counter] = temp;
            counter += 1;
            delimiter = i + 1;
        }
        if (i == len - 1){
            std::string temp = "";
            for (int j = delimiter; j < len; j++){
                temp += in[j];
            }
            res[counter] = temp;
            counter += 1;
        }
    }
    res[amount + 1] = "\0";

    return res;
}

// Exception classes

class DimensionError : public std::exception {
public:
    DimensionError(){
        std::exception();
    }

    char* what(){
        return RED "DimensionError: Dimension of arguments must match." WHITE;
    }
};

class RangeError : public std::exception {
public:
    RangeError(){
        std::exception();
    }

    char* what(){
        return RED "RangeError: Argument out of allowed range" WHITE;
    }
};

class ZeroDivisionError : public std::exception {
public:
    ZeroDivisionError(){
        std::exception();
    }
    char* what(){
        return RED "ZeroDivisionError: Division by zero not viable" WHITE;
    }
};

// Core

class Vector {

public:
    int dimension;
    float *values;

    Vector(int dim = 1, ...){

        if (dim < 0) throw RangeError();
        this->dimension = dim;

        // Here I do the trick; at above lines, I have created a float ptr.
        // Now I create a float from heap, then assign the pointer of this
        // dataset to the previous float ptr.
        float* temp_values = new float[dim];
        std::va_list val;
        va_start(val, dim);

        for (int i = 0; i < dim; i++){
            temp_values[i] = static_cast<float>(va_arg(val, double)); // 1.4013e-45;
        }
        va_end(val);
        values = temp_values;
    }


    void show() const{
        std::cout << "[ ";
        for (int i = 0; i < this->dimension; i++){
            std::cout << this->values[i] << " ";
            if (i != this->dimension - 1){
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    double length() const{
        double temp = 0;
        for (int i = 0; i < this->dimension; i++){
            temp += this->values[i] * this->values[i];
        }

        return sqrt(temp);
    }

    float dot(Vector v) const{
        if (v.dimension != this->dimension) throw DimensionError();

        float temp = 0;
        for (int i = 0; i < this->dimension; i++){
            temp += this->values[i] * v.values[i];
        }

        return temp;
    }

    Vector proj(Vector v){
        if (v.dimension != this->dimension) throw DimensionError();
        if (v.length() == 0) throw ZeroDivisionError();

        float coefficient = this->dot(v);
        coefficient /= (pow(v.length(), 2));


        return coefficient * v;
    }

    Vector unit(){
        // The reason I precalculate the length here and store it in a var
        // is that, I don't want to calculate it twice. A small optimization.
        double length = this->length();
        if (length == 0) return *this;

        return *this / length;
    }

    static Vector randVint(int dim = 2, long low = 0, long high = 10){
        // Setting limiter "high" too high will not work the intended way
        // Same goes for wide ranges

        // Low included, high excluded
        if (low == high or low > high or dim < 1) throw RangeError();

        Vector result(dim);
        long range = high - low;
        long temp;
        for (int i = 0; i < dim; i++){
            // This is to increase randomness
            temp = (Random() % range + Random() % unimportant_divisor % range + Random() % range) % range;
            result.values[i] = (temp + low);
        }

        return result;
    }

    static Vector randVfloat(int dim = 2, float low = 0, float high = 10){
        // So ratio of random integers are random floats
        if (low == high or low > high or dim < 1) throw RangeError();

        Vector result(dim);
        long range = high - low;

        long rand_temp;
        double temp;

        for (int i = 0; i < dim; i++){
            rand_temp = Random() % Random();
            long divisor = Random() % Random();

            while (divisor < rand_temp){
                divisor = Random() % Random();
            }
            temp = rand_temp * range;
            result.values[i] = (temp / divisor) + low;
        }

        return result;
    }

    static Vector randVbool(int dim = 2){
        if (dim < 1) throw RangeError();

        Vector result(dim);

        for (int i = 0; i < dim; i++){
            result.values[i] = (Random() % 2 + Random() % unimportant_divisor + Random() % 2) % 2;
        }

        return result;
    }

    Vector pop(int order = -1){
        // I cannot use the keyword "this" in the argument definitions, so I just do it here
        if (order == -1) {
            order = this->dimension-1;
        } else if (order < -1 or order >= this->dimension){
            throw RangeError();
        } else if (this->dimension == 1){
            return Vector(0);
        }

        // This function does not change the original vector
        // Simply creates another one with proper values and dimension

        Vector result(this->dimension - 1);

        for (int i = 0; i < this->dimension - 1; i++){
            if (i < order){
                result.values[i] = this->values[i];
            } else {
                result.values[i] = this->values[i + 1];
            }
        }

        return result;
    }

    Vector append(float value){
        Vector result(this->dimension + 1);

        for (int i = 0; i < this->dimension; i++){
            result.values[i] = this->values[i];
        }

        result.values[this->dimension] = value;

        return result;
    }

    Vector insert(int order = -1, float value = 0){
        if (order < -1 or order > this->dimension) throw RangeError();
        if (order == -1) order = this->dimension;

        Vector result(this->dimension + 1);

        for (int i = 0; i < this->dimension + 1; i++){
            if (i < order){
                result.values[i] = this->values[i];
            } else if (i == order) {
                result.values[i] = value;
            } else {
                result.values[i] = this->values[i - 1];
            }
        }

        return result;
    }

    Vector reverse(){
        Vector result(this->dimension);

        for (int i = 0; i < this->dimension; i++){
            result.values[i] = this->values[this->dimension - i - 1];
        }

        return result;
    }

    friend std::ostream& operator<< (std::ostream& output, Vector v){
        // same as .show()
        output << "[ ";
        for (int i = 0; i < v.dimension; i++){
            output << v.values[i] << " ";
            if (i != v.dimension - 1){
                output << ", ";
            }
        }
        output << "]";
        return output;

    }

    Vector operator+ (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        Vector result(this->dimension);
        for (int i = 0; i < this->dimension; i++){
            result.values[i] = v.values[i] + this->values[i];
        }
        return result;
    }

    Vector operator+= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            this->values[i] = v.values[i] + this->values[i];
        }
        return *this;
    }

    Vector operator- (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        Vector result(this->dimension);
        for (int i = 0; i < this->dimension; i++){
            result.values[i] = this->values[i] - v.values[i];
        }
        return result;
    }

    Vector operator-= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            this->values[i] = this->values[i] - v.values[i];
        }
        return *this;
    }

    friend Vector operator* (float c, Vector v){

        Vector result(v.dimension);

        for (int i = 0; i < v.dimension; i++){
            result.values[i] = c * v.values[i];
        }

        return result;
    }

    friend Vector operator*= (float c, Vector v){

        for (int i = 0; i < v.dimension; i++){
            v.values[i] = c * v.values[i];
        }

        return v;
    }

    friend Vector operator/ (Vector v, float c){
        if (c == 0) throw ZeroDivisionError();

        Vector result(v.dimension);

        for (int i = 0; i < v.dimension; i++){
            result.values[i] = v.values[i] / c;
        }

        return result;
    }

    friend Vector operator/= (Vector v, float c){
        if (c == 0) throw ZeroDivisionError();

        for (int i = 0; i < v.dimension; i++){
            v.values[i] = v.values[i] / c;
        }

        return v;
    }

    bool operator== (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            if (this->values[i] != v.values[i]) return false;
        }

        return true;
    }

    bool operator> (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        if (this->length() > v.length()) return true;

        return false;
    }

    bool operator>= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        if (this->length() >= v.length()) return true;

        return false;
    }

    bool operator< (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        if (this->length() < v.length()) return true;

        return false;
    }

    bool operator<= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        if (this->length() <= v.length()) return true;

        return false;
    }


    Vector operator& (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        Vector result(this->dimension);

        for (int i = 0; i < this->dimension; i++){
            result.values[i] = (static_cast<int>(v.values[i]) & static_cast<int>(this->values[i]));
        }

        return result;
    }

    Vector operator&= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            this->values[i] = (static_cast<int>(v.values[i]) & static_cast<int>(this->values[i]));
        }

        return *this;
    }

    bool operator&& (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            if (not (v.values[i] && this->values)) {
                return false;
            }
        }

        return true;
    }

    Vector operator| (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        Vector result(this->dimension);

        for (int i = 0; i < this->dimension; i++){
            result.values[i] = (static_cast<int>(v.values[i]) | static_cast<int>(this->values[i]));
        }

        return result;
    }

    Vector operator|= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            this->values[i] = (static_cast<int>(v.values[i]) | static_cast<int>(this->values[i]));
        }

        return *this;
    }

    bool operator|| (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            if (not (v.values[i] || this->values)) {
                return false;
            }
        }

        return true;
    }

    Vector operator^ (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        Vector result(this->dimension);

        for (int i = 0; i < this->dimension; i++){
            result.values[i] = (static_cast<int>(v.values[i]) ^ static_cast<int>(this->values[i]));
        }

        return result;
    }

    Vector operator^= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            this->values[i] = (static_cast<int>(v.values[i]) ^ static_cast<int>(this->values[i]));
        }

        return *this;
    }

    static float determinant(int dim, Vector v_list[]){
        // Input must be a vector list whose last element is Vector(0)
        if (dim < 1) throw RangeError();

        int amount = 0;

        for (int i = 0; i < dim; i++){
            amount += 1;
            if (v_list[i].dimension != dim) throw DimensionError();
        }

        if (dim == 1) return 1/v_list[0].values[0];
        if (dim == 2){
            return v_list[0].values[0] * v_list[1].values[1] - v_list[0].values[1] * v_list[1].values[0];
        }

        Vector sub_list[dim - 1];
        float result = 0;

        for (int i = 0; i < dim; i++){
            float factor = v_list[0].values[i];
            if (factor == 0) continue;

            for (int j = 1; j < dim; j++){
                Vector temp(dim - 1);
                int counter = 0;
                for (int k = 0;  k < dim; k++){
                    if (k != i){
                        temp.values[k - counter] = v_list[j].values[k];
                    } else {
                        counter += 1;
                    }
                }
                sub_list[j - 1] = temp;
            }

            result += Vector::determinant(dim - 1, sub_list) * pow(-1, i) * factor;
        }

        return result;
    }

    static Vector cross(int dim, Vector v_list[]){
        // dim here is the vectors' dimension
        if (dim < 2) throw RangeError();

        int amount = 0;

        for (int i = 0; i < dim - 1; i++){
            amount += 1;
            if (v_list[i].dimension != dim) throw DimensionError();
        }

        if (dim == 2){
            Vector result(2);
            // Just rotate it by 90 degrees.
            result.values[0] = -v_list[0].values[0];
            result.values[1] = v_list[0].values[1];
            return result;
        }

        Vector sub_list[dim - 1];
        Vector result(dim);

        for (int i = 0; i < dim; i++){

            for (int j = 0; j < dim - 1; j++){
                Vector temp(dim - 1);
                int counter = 0;
                for (int k = 0;  k < dim; k++){
                    if (k != i){
                        temp.values[k - counter] = v_list[j].values[k];
                    } else {
                        counter += 1;
                    }
                }
                sub_list[j] = temp;
            }

            result.values[i] = Vector::determinant(dim - 1, sub_list) * pow(-1, i);
        }

        return result;
    }

};

class Matrix {
public:
    std::string dimension;
    int m;
    int n;
    Vector *rows;
    // This is the default vector lis so that clang does not paint the line red
    static Vector *_default[1];
    // Arguments are going to be vectors just like in py version
    // This time I am not using variatic args because "Vector" is non-POD, says clang...

    Matrix(std::string dim = "0x0", Vector *v[] = _default) {
        // First argument must be in the format
        // Second argument must be a stack created pointer array.
        std::string* temp = split(dim, 'x');
        this->dimension = dim;
        this->m = std::stoi(temp[0]); // amount of rows, aka vector
        this->n = std::stoi(temp[1]); // amount of columns, aka vectors dimension

        if (this->m < 0 or this->n < 0) throw DimensionError();

        Vector* v_list = new Vector[this->m];

        for (int i = 0; i < this->m; i++){
            v_list[i] = *v[i];
            if(v_list[i].dimension != this->n) throw DimensionError();
        }

        this->rows = v_list;

    }

    void show() {
        for (int i = 0; i < this->m; i++){
            std::cout << "[ ";
            for (int j = 0; j < this->n; j++){
                std::cout << this->rows[i].values[j] << " ";
            }
            std::cout << "]" << std::endl;
        }
    }

    Matrix copy() {
        Vector *temp[this->m];

        for (int i = 0; i < this->m; i++){
            // Vector template created
            Vector *t = new Vector(this->n);
            for (int j = 0; j < this->n; j++){
                // Values filled
                (*t).values[j] = this->rows[i].values[j];
            }
            // Mem address inserted
            temp[i] = t;
        }

        Matrix result(this->dimension, temp);
        // Clean the memory up I guess
        for (Vector *v : temp){
            delete v;
        }
        return result;
    }

    friend std::ostream& operator<< (std::ostream& output, Matrix M){
        for (int i = 0; i < M.m; i++){
            output << "[ ";
            for (int j = 0; j < M.n; j++){
                output << M.rows[i].values[j] << " ";
            }
            output << "]" << std::endl;
        }

        return output;
    }

    Matrix operator+ (Matrix M) {
        // Does this algorith result in a memory leak?
        // Like a heap allocation is done here.
        // And another one is done in the initialization.

        // We create a vector list
        if (M.dimension != this->dimension) throw DimensionError();

        Vector *temp[M.m];

        for (int i = 0; i < M.m; i++){
            // Vector template created
            Vector *t = new Vector(M.n);
            for (int j = 0; j < M.n; j++){
                // Values filled
                (*t).values[j] = this->rows[i].values[j] + M.rows[i].values[j];
            }
            // Mem address inserted
            temp[i] = t;
        }
        Matrix result(M.dimension, temp);
        // Clean the memory up I guess
        for (Vector *v : temp){
            delete v;
        }
        return result;
    }

    Matrix operator+= (Matrix M) {
        if (M.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < M.m; i++){
            for(int j = 0; j < M.n; j++){
                this->rows[i].values[j] += M.rows[i].values[j];
            }
        }

        return *this;
    }

    Matrix operator- (Matrix M) {
        if (M.dimension != this->dimension) throw DimensionError();

        // We create a vector list
        Vector *temp[M.m];

        for (int i = 0; i < M.m; i++){
            // Vector template created
            Vector *t = new Vector(M.n);
            for (int j = 0; j < M.n; j++){
                // Values filled
                (*t).values[j] = this->rows[i].values[j] - M.rows[i].values[j];
            }
            // Mem address inserted
            temp[i] = t;
        }
        Matrix result(M.dimension, temp);
        for (Vector *v : temp){
            delete v;
        }
        return result;
    }

    Matrix operator-= (Matrix M) {
        if (M.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < M.m; i++){
            for(int j = 0; j < M.n; j++){
                this->rows[i].values[j] -= M.rows[i].values[j];
            }
        }

        return *this;
    }
};



#endif //VECTOR_CPP_VECTOR_H
