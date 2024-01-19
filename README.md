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

Random number generating functions are created from raw _rand()_ function of C++. 
These are then used to implement random vector generation with static methods.

Type converting functions are also added.

Some more operator overloads are included like ++, --, *=, etc.

Matrix class included, with almost all of the essential operations. Determinant is
included in the Vector class, with cross product operation. 

Matrix multiplication is included with the default textbook multiplication algorithm.

Matrices are just Vectors of Vector pointers. Their inner parameters like "size" and
"length" are hidden in the Vector class. Their "data" is just a pointer to the Vector 
of Vector pointers. Reaching the data without the [] operator is a little bit tricky
because of that, but this implementation is indeed very easy to do since the Vector
class handles all the tedious operations like resizing, etc. 

Resizing will be redone in the near future. Max size update will be 64 places.

complex class is being included. Function definitions of the complex class are designed
to use minimal RAM instead of the fastest option. Since there are 2 number in each complex
number, speed won't make a big difference.

My pathetic trial of Infinity and Undefined is also included.

Vector and Matrix classes are further expanded to include more methods, primarily 
comparison overloads.
