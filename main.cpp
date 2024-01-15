#include <iostream>
#include "vectorgebra.h"
#include <chrono>


int main() {
    srand(unsigned(int(time(0))));
    int temp[3] = {1 ,2,3};
    int temp2[3] = {3, 4, 5};
    int temp3[3] = {1, 4, -1};
    Vector<int> v(3, temp);
    Vector<int> w(3, temp2);
    Vector<int> q(3, temp3);

    Vector<double> v_list_last[10] = {Vector<double>::randVdouble(10, -3, 3),
                                     Vector<double>::randVdouble(10, -3, 3),
                                     Vector<double>::randVdouble(10, -3, 3),
                                     Vector<double>::randVdouble(10, -3, 3),
                                     Vector<double>::randVdouble(10, -3, 3),
                                      Vector<double>::randVdouble(10, -3, 3),
                                      Vector<double>::randVdouble(10, -3, 3),
                                      Vector<double>::randVdouble(10, -3, 3),
                                      Vector<double>::randVdouble(10, -3, 3),
                                      Vector<double>::randVdouble(10, -3, 3)};


    Vector<int> v_list[3];
    Vector<int> v_list2[2];
    v_list[0] = v;
    v_list2[0] = v;
    v_list2[1] = w;
    v_list[1] = w;
    v_list[2] = q;
    std::cout << v << std::endl;
    v.reverse();
    std::cout << v << std::endl;
    v.reverse();
    std::cout << v << std::endl;

    std::cout << w << std::endl;
    auto r = v.proj(w);
    std::cout << r << std::endl;
    std::cout << v * r << " ?==? " << v.len() * r.len();
    for (int i = 0; i < 10; i++) std::cout << randint(-5, 3) << std::endl;
    std::cout << Vector<int>::randVint(5, 0, 10) << std::endl;

    std::cout << Vector<int>::cross(2, v_list2) << std::endl;

    Matrix<int> m(2, 3, v_list2);

    std::cout << m << std::endl;
    std::cout << std::endl;
    m.append(q);
    std::cout << m << std::endl;
    std::cout << std::endl;
    m.pop();
    std::cout << m << std::endl;
    std::cout << std::endl;
    std::cout << m + 1 << std::endl;
    std::cout << std::endl;
    std::cout << m.transpose() << std::endl;
    std::cout << std::endl;
    std::cout << m - m << std::endl;
    std::cout << std::endl;
    std::cout << m[0][2] << std::endl;
    std::cout << m / 3<< std::endl;
    std::cout << std::endl;
    std::cout << m * m.transpose() << std::endl;
    Matrix<double> n;
    n.append(Vector<double>::randVdouble(2, 0, 2));
    n.append(Vector<double>::randVdouble(2, -2, 5));
    n.append(Vector<double>::randVdouble(2, -3, 0));
    std::cout << n * m << std::endl;
    std::cout << 0.5 * m << std::endl;
    std::cout << Matrix<float>::randMfloat(3, 3, -2, 2) << std::endl;
    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    begin = std::chrono::steady_clock::now();
    Matrix<double>::randMdouble(1000, 1000, -10, 10) * Matrix<double>::randMdouble(1000, 1000, -10, 10);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> dur{end - begin};
    std::cout << "Time of multiplication 1000x1000: " << dur.count() << std::endl;

    return 0;
}
