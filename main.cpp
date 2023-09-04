#include <iostream>
#include "Vector.h"


int main() {
    srand(unsigned(int(time(0))));
    Vector v(4, 1., 2., 3., 0.);
    Vector w(4, 1., 2., 3., 5.);

    try {
        v = v / 0;
    } catch(ZeroDivisionError& e){
        std::cout << e.what() << std::endl;
    }


    return 0;
}
