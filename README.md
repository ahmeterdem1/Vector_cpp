# Vector

The same vector library, in c++.

## General Methods

I try to stick to the same conventions
that i have used for the python version.
These include variatic argument initializations.
I don't like to use array inputs.

Variatic arguments came at a cost though, 
for now at least. Vector's values all must be
float. You need to at least put a dot after the
number. Otherwise typecasting just stops working.

I won't exlpain each method here because they are
the same as their python version. Just maybe faster.

I really would like to go deep in algorithms with
matrix multiplication and determinants though, this
is the only reason that I have decided to code this
library in C++ too.

And I would like this library to be more high level
than general C++ code. What I mean by that is like,
having exception classes and _maybe_ some built in 
logging.

## Details

There are some details that are important to know
before using the library.

### Casting

&, | and ^ operations static cast to integer before
applying the operation. Same for the assignment included
versions of those. This casting is not done for bool 
returning versions of these operators.

### Randomness

Randomness source is indifferent to a toy. I will
probably implement better randomness functions in
the future. But for now, I want it to be as simple
as it can be. Therefore I only use srand() and rand()
with a custom increased randomness implementation.

How this randomness works is that, firstly you need to
seed the source with time(0) in int main. Then you may
call the Random() function defined in the library.

What this function does is, it reseed the source with 
rand(). Then recalls rand() to generate a random number
with the new "random" seed. If you don't reseed srand()
then repeated calls of rand() will result in close numbers
incremented by a small difference in order.

To increase randomness in static class methods, I do a
little trick. I call the Random() 3 times consecutively
and add them. The reason why I am not using multiplication
is that; firstly that will result in too many bits for plain
int, secondly Random() has a tendency to return even numbers.
If I multiply I won't get any odd numbers. Since Random() may
also return odd numbers, adding them is a better choice.

And I defined a prime number that I picked randomly to
do mod operations to increase randomness. I take the
remainder from division by this prime with one of the
Random()'s. If I get an even number with Random(), then
this remainder must be odd. So one of the 3 operands to the
sum is most likely odd.

For the float number generation, I just generate 2 random
integers by calling Random() % Random(). The latter must
be bigger than the first one. When I take first / second,
I get a random float in range [0, 1). I just scale this
up to the inputted range.

Why I don't use already existing libraries?

I want to create my own.

## Speed comparisons

These are the results of speed comparisons between python
and C++. Results are obtained by doing the operation 10000
times then taking the average.

All measurements are done on the same 4d vector(s).

### Vector initialization

C++ is around 100 times faster than python. Generated vectors
are not saved to any data structure in this test.

### Basic operations

Add, subtract and multiply are all 100 times faster in C++.
Division is 110 times faster in C++.

& operation is only around 1.6 times faster in C++. A similar
pattern is observed for | operation. It is only 1.35 times
faster in C++.

However, the old 100 times is regained for xor. C++ is again
around 100 times faster there.

### Other methods

For .length(), C++ is 300 times faster.

For .unit(), C++ is 110 times faster.

.proj() is just around 20 times faster in C++.

.pop() is measured by regenerating the same vector at each
iteration. This is done because of the nature of pop in python.
C++ is measured to be around 200 times faster.

.insert() and .reverse() are not in python version

### Randoms

For .randVint(), 2d in range(10), python is 100 times faster.

For .randVbool(), 2d, python is 150 times faster.

For .randVfloat(), 2d in range(10), python is around 15 times
faster.

Python is faster in randoms because I use a custom random generation
method in C++, which is probably slower. C++ is very close to python
in floats even though I use an inner loop to generate divisor. But
still, the loop most likely never gets entered.

### Save the data

Vector initialization and savings to the memory will be the only
test here. We will generate vectors and save these in stack memory
in C++. Same goes for python. Python will use tuple as the stack type.

Python is surprisingly 100 times faster. However the standard deviation
is high. Low end is 70 times, high end is 145 times. Average looks like
to be around 100 times.

If we use lists instead of tuples, python is again 100 times slower.

### A small combined operation

A stack of vectors are created. Then these vectors are summed together.

C++ is only around 25 times faster in this combined operation program.

I still haven't coded the matrix part yet but it seems like C++ is fast
enough to be worth coding in. But python is still fast enough.

