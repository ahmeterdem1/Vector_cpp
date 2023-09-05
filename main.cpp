#include <iostream>
#include "Vector.h"
#include <chrono>


int main() {
    srand(unsigned(int(time(0))));
    std::chrono::time_point<std::chrono::system_clock> begin, end;
    Vector v(4, 1., 2., 3., 0.);
    Vector w(4, 2., 3., 0., 1.);

    begin = std::chrono::system_clock::now();

    Vector *v_list[10000];
    for (int i = 0; i < 10000; i++) {
        Vector v(4, 1., 2., 3., 0.);
        v_list[i] = &v;
    }

    Vector sum(4, 0., 0., 0., 0.);
    for (int i = 0; i < 10000 ; i++){
        sum += *v_list[i];
    }
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> difference = end - begin;

    std::cout << "Average time is: " << (difference.count() / 10000) << std::endl;

    return 0;
}
