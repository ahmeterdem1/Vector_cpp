#include <iostream>
#include "vectorgebra.h"
#include <chrono>


int main() {
    srand(unsigned(int(time(0))));
    int temp[3] = {1 ,2,3};
    int temp2[3] = {3, 4, 5};
    Vector<int> v(3, temp);
    Vector<int> w(3, temp2);
    Vector<int> v_list[2];
    v_list[0] = v;
    v_list[1] = w;
    std::cout << v << std::endl;
    std::cout << w << std::endl;
    auto r = v.proj(w);
    std::cout << r << std::endl;
    std::cout << v * r << " ?==? " << v.len() * r.len();
    for (int i = 0; i < 10; i++) std::cout << randint(-5, 3) << std::endl;
    std::cout << Vector<int>::randVint(5, 0, 10) << std::endl;
    return 0;
}
