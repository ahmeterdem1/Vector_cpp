#include <iostream>
#include "vectorgebra.h"
#include <chrono>


int main() {
    srand(unsigned(int(time(0))));
    int temp[3] = {1 ,2,3};
    Vector<int> v(3, temp);
    std::cout << v << std::endl;
    return 0;
}
