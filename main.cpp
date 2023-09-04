#include <iostream>
#include "Vector.h"


int main() {
    srand(unsigned(int(time(0))));
    Vector v(4, 1., 2., 3., 0.);
    Vector w(4, 1., 2., 3., 5.);

    for (int i = 0; i < 100; i++){
        std::cout << Vector::randVfloat(2, -5, 5) << std::endl;
    }


    return 0;
}
