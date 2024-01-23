#include <iostream>
#include "include/vectorgebra.h"
#include <chrono>
#include <functional>


double polynomial(double x) {
    return pow(x, 2) - 2 * x + 1;
}


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
    std::cout << m.determinant() << std::endl;
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

    /*
    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    begin = std::chrono::steady_clock::now();
    Matrix<double>::randMdouble(1000, 1000, -10, 10) * Matrix<double>::randMdouble(1000, 1000, -10, 10);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> dur{end - begin};
    std::cout << "Time of multiplication 1000x1000: " << dur.count() << std::endl;
    */
    std::cout << v << std::endl;
    std::cout << Matrix<int>::identity(3) * v << std::endl;
    std::cout << v.cumsum() << std::endl;
    std::cout << (v == v) << std::endl;
    std::cout << (v >= w.toFloat()) << std::endl;
    std::cout << m.cumsum() << std::endl;
    std::cout << m.sum() << std::endl;
    std::cout << v.sum() << std::endl;
    auto c = complex<int>(1, -2);
    std::cout << c / 2.2 << std::endl;
    std::cout << sqrt(5.5, 15) << std::endl;

    // cursed loop
    for (float k in Range<float>(3, 5, 0.1)) {
        std::cout << k << std::endl;
    }

    std::cout << e(2) << std::endl;
    std::cout << sin(30, 10) << std::endl;
    std::cout << cos(30, 10) << std::endl;

    Vector<double> dlist[3];
    dlist[0] = v.toDouble();
    dlist[1] = w.toDouble();
    dlist[2] = q.toDouble();
    auto new_list = Vector<double>::spanify(3, dlist);
    std::cout << Vector<double>::doesSpan(3, new_list) << std::endl;

    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    auto M = Matrix<int>::randMint(5, 5, -5, 5);
    std::cout << "Generation done" << std::endl;
    begin = std::chrono::steady_clock::now();
    auto det = M.determinant();
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> dur{end - begin};
    std::cout << "Time of determinant: " << dur.count() << std::endl;
    std::cout << det << std::endl;
    std::cout << log2(32, 30) << std::endl;
    std::cout << Sum<double, double>([] (double x) -> double {
        return 1 / pow(2, x);
    }, 0, 20) << std::endl;
    std::cout << combination(5, 1) << std::endl;
    std::cout << v << std::endl;
    std::cout << v.map<double>(polynomial) << std::endl;

    auto logger = Logger("root");
    logger.setLevel(DEBUG);
    logger.debug("debug message");
    logger.info("info message");
    logger.warning("warning message");
    logger.critical("critical message");
    logger.fatal("fatal message");

    double row1[3] = {1, 0, 2};
    double row2[3] = {3, 2, 2};
    double row3[3] = {0, 2, 2};

    Vector<double> v1(3, row1);
    Vector<double> v2(3, row2);
    Vector<double> v3(3, row3);

    Vector<double> list[3] = {v1, v2, v3};
    Matrix<double> N(3, 3, list);


    double measurement = 0;

    /*
    for (int i = 0; i < 1000; i++) {
        auto Q = Matrix<double>::randMdouble(500, 500, -5, 5);
        begin = std::chrono::steady_clock::now();
        auto inv = Q.inverse("gauss");
        end = std::chrono::steady_clock::now();
        dur = end - begin;
        measurement += dur.count();
    }
    */

    auto Q = Matrix<double>::randMdouble(500, 500, -5, 5);
    begin = std::chrono::steady_clock::now();
    auto inv = Q.inverse("gauss");
    end = std::chrono::steady_clock::now();
    dur = end - begin;
    measurement += dur.count();

    printf("Average time for 500x500 matrix inversion is %f seconds.", measurement);
    return 0;
}
