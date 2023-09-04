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