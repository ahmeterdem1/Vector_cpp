#include <iostream>
#include "Vector.h"
#include <chrono>


int main() {
    srand(unsigned(int(time(0))));

    Vector *v_list = new Vector[3];

    Vector v(4, 1., 0., 0., 0.);
    Vector w(4, 0., 1., 0., 0.);
    Vector q(4, 0., 0., 0., 1.);
    Vector p(4, 1., 2., 3., 4.);

    v_list[0] = v;
    v_list[1] = w;
    v_list[2] = q;

    std::cout << Vector::cross(4, v_list) << std::endl;

    return 0;
}
