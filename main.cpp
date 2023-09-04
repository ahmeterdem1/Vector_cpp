#include <iostream>
#include "Vector.h"


int main() {
    srand(unsigned(int(time(0))));
    Vector v(4, 1., 2., 3., 0.);
    Vector w(4, 1., 2., 3., 5.);

    w = v.proj(w);

    std::cout << Random() << std::endl;
    return 0;
}
