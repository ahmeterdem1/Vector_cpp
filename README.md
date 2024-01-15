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

