# Vectorgebra

The same vectorgebra library, in C++.

## Notes on the Latest Changes

I have started to write this library from the very beginning. I am now implementing
smart arrays as vectors. Most of the Vector class in Vectorgebra is done here. Random
generations will be looked at later. I will be more careful with the implementations.
I will try to make them as efficient as possible. Then we will do all the comparisons
again with python3.9, python3.11 and pypy.

Stay tuned for more.

## Recent updates

After the library is complete to a decent level, a proper explanation for each method 
and function will be given here.

First impressions of the code; determinant calculation of a 500x500 matrix is measured 
to be ~4600 times faster in Vectorgebra/C++ than Vectorgebra/Python3.11. Determinant
is of course calculated via the echelon method. Analytic would never return regardless
of the language.