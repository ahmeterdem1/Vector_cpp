#ifndef VECTOR_CPP_VECTORGEBRA_H
#define VECTOR_CPP_VECTORGEBRA_H

#include <ostream>
#include <type_traits>
#include <functional>
#include "vector.h"
#include "matrix.h"
#include "./complex.h"

typedef Vector<int> iVector;
typedef Vector<float> fVector;
typedef Vector<double> dVector;
typedef Vector<bool> bVector;
typedef Vector<long> lVector;
typedef Vector<short> sVector;

typedef Matrix<int> iMatrix;
typedef Matrix<float> fMatrix;
typedef Matrix<double> dMatrix;
typedef Matrix<bool> bMatrix;
typedef Matrix<long> lMatrix;
typedef Matrix<short> sMatrix;

typedef complex<int> icomplex;
typedef complex<float> fcomplex;
typedef complex<double> dcomplex;
typedef complex<bool> bcomplex;
typedef complex<long> lcompelx;
typedef complex<short> scomplex;

#endif //VECTOR_CPP_VECTORGEBRA_H