# Vectorgebra

The same vectorgebra library, in C++.

## Project Details

This project is aimed to use exactly the same algorithms as Vectorgebra, and transport
those into C++. Algorithms and details may differ, but the general idea is _always_ the
same. 

In this README file, the Python Vectorgebra library will be referred as "Vectorgebra/Python",
and the C++ implementation will be referred as "Vectorgebra/C++".

Examples for the code usage can be found in the main.cpp. It has examples for the most of
the important parts of the code.

First impressions of the code; determinant calculation of a 500x500 matrix is measured
to be ~4600 times faster in Vectorgebra/C++ than Vectorgebra/Python3.11. Determinant
is of course calculated via the echelon method. Analytic would never return regardless
of the language.

## File Structure

There are a total of 9 header files. _vectorgebra.h_ is the file that collects all the 
others. It also has function definitions for compound functions. Compound functions are
functions that require multiple objects/functions/etc from multiple header files that are
chained together. Those definitions cannot be done in any other header file; as this creates
an import loop, which prevents all the code below the _include_ statement from working.

5 of the other 6 header files are chained together. The chain is:

`exceptions.h -> helpers.h -> functions.h -> vector.h -> matrix.h -> graph.h`

_graph.h_ is then included from _vectorgebra.h_. _compelx.h_ is only connected to the
main vectorgebra header. It still can be used with objects from the header chain. You
can, for example, create complex valued vectors and matrices. _node.h_ is included from
_graph.h_.

Each files content will be explained in order.

<br>

## Exceptions

There are 4 exception classes. IndexError, RangeError, DimensionError, ZeroDivisionError.
Unlike Vectorgebra/Python, there are no hints that can be given as an argument to the
exceptions. Also, there are no choice codes for the DimensionError. _.what()_ method
returns a general, not much explanatory text about the raised exception.

<br>

## Helpers

Helpers can be defined as anything that may be used in other Vectorgebra/C++ implementations,
but are completely separate from those by their idea and purpose. Helpers include constant
definitions. Those are exactly the same as Vectorgebra/Python. This file also has an 
implementation of logger, which works very similar to the Vectorgebra/Python one. Definitions
in the file include terminal color codes for POSIX environment. Those are used by the logger,
to display colored text depending on the logged level of the message.

Those colors will only work for POSIX users. Windows users should mind this lack of implementation.

The loggers format is preset, which is almost exactly the same as the one used in Vectorgebra/Python.
I tidied it up a little bit, it mostly remains unchanged. 

Levels for the logger are defined as DEBUG, INFO, WARNING, CRITICAL, FATAL. Default setting is INFO.
You can set level by _.setLevel()_ method. Examples of use can be found in the main.cpp file.
The same goes for the format. You can set it via _.setFormat()_. Argument must be an
std::string, whose .c_str() goes as the first argument to the _printf_ function. 
Apply your format accordingly.

You can get level and format by _.getLevel()_ and _.getFormat()_. You can print messages via
e.g. _logger.debug()_, _logger.warning()_, etc.

### Ranges

This header file has an iterator wrapper class that works exactly the same as Vectorgebra/Python;
Range. It is a template class, as most of the classes defined in this library. So different
from the Python implementation, you need tp specify the types to the template. You can still
give custom steps. I have defined ":" as "in", so you can generate loops like below:

`for (double a in Range<double>(-5, 5, 0.05) {yourCode;})`

Why not make it more Pythonic if we were to add Ranges already?

### Not a Number

After much consideration, i have implemented infinities and the undefined in the same class.
It acts like a 3 state variable. Just as a boolean, but with 3 states instead of 2. The first
2 states are, in order, -infinity and +infinity. The third state is _undefined_. 

I have created an "undefined" object at the beginning of the header file. All the functions 
and method that may return an undefined, returns this exact object. So it acts like the
Python "None". It acts like there is just one of it. But this is just an illusion of the
code, you can create as many undefineds as you want.

I have created 2 wrapper functions for this system to act like as there were 2 different
classes for infinities and the undefined. "Infinity(_bool_)" returns a "NaN" that is of
state infinity. "Undefined()" returns the <b>global</b> undefined object. Example usages,
again, can be found at the main.cpp file.

<br>

## Functions

This file has the implementations of general numerical methods implementations of mathematical
and analytical functions. The biggest difference from Vectorgebra/Python is that, i 
implement my own random generating functions. I just use the built-in "rand()", and
go from it. All random in-range number generators start from the function _randf_, 
which generates a random float in range 0 to 1. This functions runtime is random,
therefore it is kind of cryptologically safe for timing. This goes for all the other
random functions, except _randbool_.

IMPORTANT POINT. Distributions of the random number generating functions used here are
NOT close to what it is supposed to be. Those implementations are just experiments.
And also, there are no normal distribution generating functions yet. Only wanna-be
uniforms. You can normalize the distributions yourself if you want.

Absolute value function is not redefined. All trigonometrics and logarithms are defined.
The precision method of using the "resolution" argument stays the same. Its default
values are literally the same as Vectorgebra/Python. Precision is not as good as 
Vectorgebra/Python for inverse trigonometrics, regardless of the resolution's value.

Except the functions, that accept callables as an argument, all the other aspects of 
this part of the library is literally the same as Vectorgebra/Python. I even recommend
you at this point checking the original [vectorgebra](https://github.com/ahmeterdem1/Vector) 
page out for more algorithmic detail.

Callable accepting functions, are templates. Treat them as accordingly. You probably need
to specify both the argument type and return type of your callable. Your callable
must be accepting only a single argument, which was also assumed in the implementations
of Vectorgebra/Python. If there must be more arguments to your function, specify
default values for those. Because you won't be passing them through Vectorgebra/C++...

Functions that work on data, (only minimum and maximum for now) are also templates.
You need to specify your data type. Any other templated function of this header
returns double. But these statistical functions return the type of your template.
They also need the pointer to your data array, and the amount of data in that array.
No variable amount of arguments implemented in this library, unlike the past
trials of Vectorgebra/C++ implementations.

<br>

## Vectors

Vectors are a custom implementation of smart arrays. They are a template class.
No destructor is defined for the class. It is up to your compiler to 
decide what to do with it, FOR NOW. However, there is a _.clear()_ method that
deletes all heap allocated memory in your vector. If in your function definitions,
you happen to use temporary vector objects, apply .clear() before returning
to prevent memory leaks. If you are returning those vectors, do not clear them.
This is because a Vector object holds the pointer to the data array as the
only kind of "data" information. So if you return a Vector object, the pointers
value gets copied. No memory leak occurs. You can still delete this exact same
memory later in your code, since the memory address that this pointer holds
exited the scope by being copied. This is also what makes defining a destructor
a tad complicated. Defining a destructor clears the data at the copied address.
Not defining a destructor lets the memory address to escape the scope, which
is extremely useful in our case. And again, this escape prevents memory leaks.

All default mathematical operations are overloaded just like Vectorgebra/Python. 
Except one case, they are all the same as Vectorgebra/Python. * operation is 
overloaded here between 2 vectors unlike Vectorgebra/Python. It is an alias 
for scalar multiplication. So, don't chain those between more than 2 vectors.

Bitwise operations (and, or, xor, etc.) are NOT overloaded here, and will not be
overloaded. I don't think they are useful for any purpose.

Type conversion functions do not convert self. They return a brand new converted
Vector object, which should be subject to proper _.clear()_ applications by
the user.

An extra method is "__pivot()". It returns the index of first non-zero element
in the Vector, which is the "pivot" of the "row". It is used in echelon form
algorithms in matrices.

Dot product, spanify and doesSpan (<-- see the name change from Vectorgebra/Python)
all are subject to the same floating point errors specified in Vectorgebra/Python.
Check the page for [vectorgebra](https://github.com/ahmeterdem1/Vector) for more 
detail.

All static methods are also templates. These templates do not serve any purpose 
for the code, but you may use it as type distinctors for more readability in
your code.

.map() and .filter() are templates on their own, aside being templates as any other
method in this class. They accept functions, assuming they only require a single
argument. Filtering accepts a callable that _only_ has a bool return type. Both
methods accept functions that has argument type the same as applied vector, which
comes from the classes general template.

<br>

## Matrices

Matrix class is a template class, which acts like a wrapper to Vector class. No
memory management algorithms have been implemented for this class, as it is
indeed just a wrapper. Memory managing methods just make class to Vector methods.

The same memory leak conditions apply for matrices too. Matrices data information,
is a pointer to a list of Vector pointers. Those pointers all point to vectors,
which each holds pointers to data arrays. It is a little bit complicated, too much
pointer stuff, but almost all algorithms follow the same code structure for 
manipulating data. This similarity makes implementations easier than it seems.

All algorithms are designed to be the most efficient for high dimensional matrices.
Pointer copying occurs in almost each algorithm. The copied pointer is the pointer
to the row Vector. This makes reaching the data in the RAM just a little bit faster.
This is because at each iteration, you need to first find the pointer to the vector,
then find the data at the wanted index. There are a lot of indirection calls made
at each iteration. Pointer-to-row copying shortens this indirection process. It is
measured to be around %2.5 faster for 500x500 random double valued matrix multiplication.

For the most important operations; determinant and inverse, determinant is completed exactly the
same as Vectorgebra/Python algorithmwise. Iterative and Gauss Jordan methods exist for the inverse for now.
I don't plan to code the analytical method for inverses. It is completely unnecessary, 
and annoyingly slow. Gauss-Jordan is more than good enough for almost any inverse operation. 
I implemented the iterative method only for its algorithmic simplicity. A numerical methods 
library mostly don't need more than one method of calculation, if that one is good enough 
for any possible case. I will implement the Neumann method though. It might be much faster 
than all others for the right conditioned matrices. This is the only valid reason for implementing
more than one algorithm for the same task. Conditional performance. It may be
performing better for some specific conditions that might be desirable for the user.

Echelon forms algorithm might be a little different. With using pivots, we sort
the rows via bubble sort. Both before and after the core algorithm that reduces
the rows. And also, differently from Vectorgebra/Python, echelon method accepts
non-square matrices.

There is a __pivots() method, that returns an integer pointer. This pointer points
to a heap allocated array, that holds the pivots indexes for each row in the
matrix, in order.

<br>

## Graphs

Graphs are a complicated data structure that directly simulate a graph structure in
the RAM with linked nodes. Each node, vertex, contains a linked list of all vertices.
This list also contains the host node itself. There is no disconnected node. Disconnections
are declared via 0-weight connections. This is the same practice from the Python counterpart.

Each node also has a weight list that corresponds to the connection list by order. This is
again the same practice from the Python version. However, unlike Vectorgebra/Python, 
a Graph object does not hold its adjacency matrix. It is generated when requested.

Both directed and undirected graphs are supported.

Graphs are still incomplete. Due to their complicated structure, their development will
probably take longer than the others. However, you can find a demo in the main.cpp.

## Complex Numbers

The implementation is very basic here, compared to all other aspects of this library.
There is no heap allocation anywhere in this header file. So use those as you wish.
All methods and functions are algorithmically the same as Vectorgebra/Python. So,
for more detail, check its [page](https://github.com/ahmeterdem1/Vector) out.

## Core Vectorgebra

This header has type definitions for each general template of each class. It also has
the _reshape_ function. It is not a method here, but a separate function. The reason
for that is, it is a compound method. Because of the import loop, i cannot define it
in any other header. In the future, there will be more functions in this header

Complex conjugation for vectors and matrices are coded here.

Frobenius product, norm and conjugations also exist here. Since conjugations require 
the complex class to be defined, this file is the only place that they could be used.

Multithreaded functions will be coded here. The first example is matrix multiplication.
Threaded matrix-matrix and matrix-vector multiplication is essential for performance
computing. Threaded functions provided here get much much faster as the dimension
increases. Algorithms used are different from Vectorgebra/Python, but are similar.

Vectorgebra/Python only included threaded matrix multiplication for matrix-matrix
multiplications. Vector counterpart does not exist there. Here, it well exists.
Its algorithm is the same for the case that, second matrix is nx1 dimensional
at Vectorgebra/Python. Rows of the first matrix are grouped by index. Then this
information is carried onto threads where related dot products are done and
written to the target memory. The thread count is defined by the log2 of row count.

Matrix-matrix multpilication is very similar to above. Unlike Vectorgebra/Python, 
both rows of the first matrix and columns of the second matrix are grouped. At
Python counterpart, only rows of the first matrix were grouped and processed
in threads. This generated a worse case at square matrices which was slower than
basic multiplication. Here, the program works much more efficiently by applying 
grouping to both operands.

Below graphs describe the features of threaded and linear multiplication algorithms
very well. 

Matrix - Vector multiplication:

![matrix - vector](https://github.com/ahmeterdem1/Vector_cpp/blob/main/mv.png)

Except some statistical anomalies which are probably due to ill conditioned matrices,
threaded multiplication is much faster at high dimensions. Only square matrices are
picked, matrices and vectors used for comparison are the same.

Matrix - Matrix multiplication:

![matrix - matrix](https://github.com/ahmeterdem1/Vector_cpp/blob/main/mm.png)

Again, only square matrices are picked. Except for very low dimensional matrices, 
threaded version is always superior.

_solve_ function is algorithmically a little bit different from Python counterpart.
Firstly, the treatment of resolution parameter is different. A point may be flagged
when the functions absolute value at said point is lower than square root of the
resolution. Threads may add the value if the functions absolute value is lower than
the resolution.

This means that for a precise point to get flagged, you might need to increase 
resolution. But to get a more precise "x", you need to decrease the resolution.

Threads utilize a global lock. There is no thread id for data collection here. Instead,
when a thread obtains the lock, it appends to the target vector, then releases the lock.

By idea, this might be slower than collection by thread id. But there is no way to know
how many zeroes we will find. We need a resizable dynamic data structure, which a
Vector<double> is. Using a hashmap is always a possible choice. But I believe that it
will be slower than just appending to a smart array. The only time loss here is that
you have to add procedurally.

<br>

## General Notes

### How to keep memory safe

Don't code like:

`std::cout << Matrix<int>::randMint(5, 5, 0, 5) * Matrix<int>::randMint(5, 5, 0, 5) << "\n";"`

You just lost pointers to 2x8x8xsizeof(int) bytes of heap allocated memory. (8 because the smart
array will allocate 8 spaces for 5 length. The next power of 2 until reaching 64.) Good luck 
finding them!

This is not a problem of having no destructors. Above code is equivalent to using malloc() 
and not copying its return value to a variable. Don't do it. I don't use smart pointers. 
I should have maybe, but here we are. Basic pointers are faster, and they require careful 
handling.

The consistent rule of memory allocation in this library is that, except complex class,
every _Matrix-Vector-Pointer_ returning _const_ labeled function/method allocates heap
memory that you will have to delete manually.

Apply functional programming patterns to your code. Go step by step. Always hold the
return information of functions.

<hr>

Naming convention is consistently Camel Case here. It was mostly Camel Case in the Vectorgebra/Python,
but it was not consistent.
