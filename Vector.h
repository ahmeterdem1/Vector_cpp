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

int Random(){
    // For this function to be useful, srand() must be
    // seeded in the main with time(0) or something.
    // This is only for limited use, obviously.

    srand(rand());
    int next_random = rand();
    return next_random;
}

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

class Vector {

public:
    int dimension;
    float *values;

    Vector(int dim, ...){

        if (dim < 1) throw RangeError();
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

};

#endif //VECTOR_CPP_VECTOR_H
