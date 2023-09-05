#include <iostream>
#include "Vector.h"
#include <chrono>


int main() {
    srand(unsigned(int(time(0))));
    std::chrono::time_point<std::chrono::system_clock> begin, end;
    Vector w(4, 2., 3., 0., 1.);

    begin = std::chrono::system_clock::now();

    for (int i = 0; i < 10000; i++){
        Vector v = Vector::randVfloat(2, 0, 10);
    }
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> difference = end - begin;

    std::cout << "Average time is: " << (difference.count() / 10000) << std::endl;

    return 0;
}
