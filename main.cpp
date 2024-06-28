#include <iostream>
#include "include/vectorgebra.h"
#include <chrono>
#include <functional>
#include <fstream>


double polynomial(double x) {
    return pow(x, 2) - 2 * x + 1;
}


int main() {
    srand(unsigned(int(time(0))));

    std::cout << "test" << std::endl;
    Vector<int> v = Vector<int>::randVint(3, 0, 5);
    std::cout << v << std::endl;
    v.clear();

    return 0;
}
