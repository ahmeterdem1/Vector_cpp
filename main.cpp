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
    /*
    int temp[3] = {1 ,2,3};
    int temp2[3] = {3, 4, 5};
    int temp3[3] = {1, 4, -1};
    Vector<int> v(3, temp);
    Vector<int> w(3, temp2);
    Vector<int> q(3, temp3);


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
    std::cout << Vector<int>::randVint(5, 0, 10) << std::endl; // Memory leak

    std::cout << Vector<int>::cross(2, v_list2) << std::endl; // Memory leak

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
    std::cout << m + 1 << std::endl; // Memory leak
    std::cout << std::endl;
    std::cout << m.transpose() << std::endl; // Memory leak
    std::cout << std::endl;
    std::cout << m - m << std::endl; // Memory leak
    std::cout << std::endl;
    std::cout << m[0][2] << std::endl;
    std::cout << m / 3<< std::endl; // Memory leak
    std::cout << std::endl;
    std::cout << m * m.transpose() << std::endl; // Memory leak
    Matrix<double> n;
    n.append(Vector<double>::randVdouble(2, 0, 2));
    n.append(Vector<double>::randVdouble(2, -2, 5));
    n.append(Vector<double>::randVdouble(2, -3, 0));
    std::cout << n * m << std::endl; // Memory leak
    std::cout << 0.5 * m << std::endl; // Memory leak
    std::cout << Matrix<float>::randMfloat(3, 3, -2, 2) << std::endl; // Memory leak
    */


    /*
    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    begin = std::chrono::steady_clock::now();
    Matrix<double>::randMdouble(1000, 1000, -10, 10) * Matrix<double>::randMdouble(1000, 1000, -10, 10);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> dur{end - begin};
    std::cout << "Time of multiplication 1000x1000: " << dur.count() << std::endl;
    */

    /*
    std::cout << v << std::endl;
    std::cout << Matrix<int>::identity(3) * v << std::endl;
    std::cout << v.cumsum() << std::endl; // Memory leak
    std::cout << (v == v) << std::endl;
    std::cout << (v >= w.toFloat()) << std::endl; // Memory leak
    std::cout << m.cumsum() << std::endl;
    std::cout << m.sum() << std::endl;
    std::cout << v.sum() << std::endl;
    auto c = complex<int>(1, -2);
    std::cout << c / 2.2 << std::endl;
    std::cout << sqrt(5.5, 15) << std::endl;

    // cursed loop
    for (float k : Range<float>(3, 5, 0.1)) {
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
    std::cout << v.map<double>(polynomial) << std::endl; // Memory leak

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

    Matrix<double> inv;
    Matrix<double>* res;
    double measurement = 0;
    for (int i = 0; i < 1000; i++) {
        auto Q = Matrix<double>::randMdouble(20, 20, -5, 5);
        begin = std::chrono::steady_clock::now();
        //inv = Q.inverse("gauss");
        res = Q.QR();
        end = std::chrono::steady_clock::now();
        res[0].clear();
        res[1].clear();
        delete[] res;
        dur = end - begin;
        measurement += dur.count();
    }
    printf("Average time of 20x20 matrix qr decomposition is %f seconds.\n", measurement / 1000);

    auto qr = M.QR();

    std::cout << M << std::endl << std::endl;
    std::cout << qr[0] << std::endl << std::endl;
    std::cout << qr[1] << std::endl << std::endl;
    std::cout << qr[0] * qr[1] << std::endl << std::endl;

    auto eigs = M.eigenvalue();
    for (int i = 0; i < M.a; i++) { // Length of the list is equal to the dimensions of the matrix associated.
        std::cout << *(eigs + i) << std::endl;
    }

    auto sub = M.submatrix(0, 3, 1, 5);
    std::cout << sub << std::endl;
    */

    Matrix<double> testmatrix;
    Matrix<double> testvector;
    Matrix<double> mul1;
    Matrix<double> mul2;
    std::chrono::time_point<std::chrono::steady_clock> begin, end;
    std::chrono::duration<double> dur;

    double t_val;
    for (int i = 10; i < 500; i++) {
        testmatrix = Matrix<double>::randMdouble(i, i, -5, 5);
        testvector = Matrix<double>::randMdouble(i, i, -5, 5);

        begin = std::chrono::steady_clock::now();
        mul1 = testmatrix * testvector;
        end = std::chrono::steady_clock::now();
        dur = end - begin;
        std::cout << dur.count() << std::endl;


        begin = std::chrono::steady_clock::now();
        mul2 = matmul(testmatrix, testvector);
        end = std::chrono::steady_clock::now();
        dur = end - begin;
        std::cout << dur.count() << std::endl;

        testvector.clear();
        testmatrix.clear();
        mul1.clear();
        mul2.clear();
    }


    /*
    auto mat1 = Matrix<double>::randMdouble(5, 5, -5, 5);
    auto mat2 = Matrix<double>::randMdouble(5, 5, -5, 5);
    std::cout << mat1 << std::endl << std::endl;
    std::cout << mat2 << std::endl << std::endl;
    auto mulmat1 = mat1 * mat2;
    auto mulmat2 = matmul(mat1, mat2);
    std::cout << mulmat1 << std::endl << std::endl;
    std::cout << mulmat2 << std::endl << std::endl;

    //std::cout << mul1 << std::endl;
    //std::cout << mul2 << std::endl;

    // Memory cleaning from left over objects
    // Lines that have "// Memory Leak" at the end
    // have memory leaks that cannot be resolved.
    // Reasons and how to avoid it is explained
    // in the README.md extensively.

    delete[] eigs;
    qr[0].clear();
    qr[1].clear();
    sub.clear();
    M.clear();
    N.clear();
    n.clear();
    v.clear();
    w.clear();
    q.clear();
    r.clear();
    v1.clear();
    v2.clear();
    v3.clear();
    */
    return 0;
}
