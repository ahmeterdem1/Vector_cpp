#include <iostream>
#include "../include/vectorgebra.h"
#include <chrono>

int main() {
    srand(unsigned(int(time(0))));

    vector_int8 testv = vector_int8::one(1000000);
    Vector<int> testv2 = Vector<int>::one(1000000);  // Gives errors with int8_t because recognizes is as "char"
                                                          // comparisons should still be OK since they are not floats

    auto begin = std::chrono::steady_clock::now();
    testv2 = testv2 + testv2;
    auto end = std::chrono::steady_clock::now();
    auto dur1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    begin = std::chrono::steady_clock::now();
    testv = testv + testv;
    end = std::chrono::steady_clock::now();

    auto dur2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    std::cout << "Accelerated sum operation took: " << dur2.count() << " nanoseconds." << std::endl;
    std::cout << "Non-accelerated sum operation took: " << dur1.count() << " nanoseconds." << std::endl;

    //std::cout << "Time gain is: " << (dur2 / dur1) << " times." << std::endl;
    printf("Time gain is: %f times\n", (double(dur1.count()) / double(dur2.count())));

    return 0;
}
