//
// Created by AHMET ERDEM on 4.09.2023.
//

#include <iostream>
#include <cmath>

#ifndef VECTOR_CPP_VECTOR_H
#define VECTOR_CPP_VECTOR_H

class DimensionError : public std::exception {
public:
    DimensionError(){
        std::exception();
    }

    char* what(){
        return "Dimension of arguments must match.";
    }
};

class ZeroDivisionError : public std::exception {
public:
    ZeroDivisionError(){
        std::exception();
    }
    char* what(){
        return "Division by zero not viable";
    }
};

class Vector {

public:
    int dimension;
    float *values;

    Vector(int dim, ...){
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

    void show(){
        std::cout << "[ ";
        for (int i = 0; i < this->dimension; i++){
            std::cout << this->values[i] << " ";
            if (i != this->dimension - 1){
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    double length(){
        double temp = 0;
        for (int i = 0; i < this->dimension; i++){
            temp += this->values[i] * this->values[i];
        }

        return sqrt(temp);
    }

    float dot(Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        float temp = 0;
        for (int i = 0; i < this->dimension; i++){
            temp += this->values[i] * v.values[i];
        }

        return temp;
    }

    friend std::ostream& operator<< (std::ostream& output, Vector v){
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

    Vector operator* (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        Vector result(this->dimension);

        for (int i = 0; i < this->dimension; i++){
            result.values[i] = this->values[i] * v.values[i];
        }

        return result;
    }

    Vector operator*= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            this->values[i] = this->values[i] * v.values[i];
        }
        return *this;
    }

    Vector operator/ (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        Vector result(this->dimension);

        for (int i = 0; i < this->dimension; i++){
            if (v.values[i] == 0) throw ZeroDivisionError();
            result.values[i] = this->values[i] / v.values[i];
        }

        return result;
    }

    Vector operator/= (Vector v){
        if (v.dimension != this->dimension) throw DimensionError();

        for (int i = 0; i < this->dimension; i++){
            if (v.values[i] == 0) throw ZeroDivisionError();

            this->values[i] = this->values[i] / v.values[i];
        }
        return *this;
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



};


#endif //VECTOR_CPP_VECTOR_H
