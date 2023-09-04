#include <iostream>
#include "Vector.h"


int main() {
    Vector v(4, 1., 2., 3., 0.);
    Vector w(4, 1., 2., 3., 5.);
    v.show();
    std::cout << v / w << std::endl;
    return 0;
}
