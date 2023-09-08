#include <iostream>
#include "Vector.h"
#include <chrono>


int main() {
    srand(unsigned(int(time(0))));

    Vector *v_list[3];

    Vector v(3, 1., 0., 8.);
    Vector w(3, 0., 1., 0.);
    Vector q(3, 0., 0., 1.);
    Vector p(3, 1., 2., 3.);

    v_list[0] = &v;
    v_list[1] = &w;
    v_list[2] = &q;

    // m and n are the same matrices.
    // Change on one will result in change on the other.
    Matrix m("3x3", v_list); // They are created from the same list
    Matrix n("3x3", v_list); // Change on one of the member vectors will result in change on the matrix too.
    Matrix c = m.copy();
    m += n;
    v += w;
    std::cout << c << std::endl; // Remains untouched.
    std::cout << m << std::endl; // Changed.
    std::cout << n << std::endl; // Changed because it is literally the same as m.
    // m and n also get affected by vectors changes.
    // c still remains untouched.


    std::cout << c.determinant() << std::endl; // determinant

    std::cout << m.transpose() << std::endl; // transpose

    std::cout << Matrix::identity(3) * c << std::endl;
    std::cout << c << std::endl;

    std::chrono::time_point<std::chrono::system_clock> begin, end;

    Matrix *m_list[101];
    Matrix I = Matrix::identity(20);

    for (int i = 0; i < 101; i++){

        Matrix temp = Matrix::randMint(20, 20, 0, 100).copy();
        m_list[i] = &temp;
    }

    begin = std::chrono::system_clock::now();

    for (int i = 0; i < 101; i++){
        I = I * (*m_list[i]);
    }

    end = std::chrono::system_clock::now();

    std::chrono::duration<double> duration;

    duration = end - begin;

    std::cout << "C++ time is: " << duration.count() << std::endl;


    return 0;
}
