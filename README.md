This repository contains code accompanying my Master's thesis "Improved inapproximability of Max-Cut through Min-Cut".
The main result of the thesis is that for sufficiently small ε > 0 it is NP-hard to distinguish between graphs in which the maximum cut contains at least a fraction 1-ε of all edges and graphs in which the maximum cut contains at most a fraction 1-1.4568ε of all edges.

The result is based on the computation of a minimum cut in a particular graph, and the code in this repository constructs that graph and computes the size of the minimum cut using the LEMON graph library.

To run this code, you first need to go to http://lemon.cs.elte.hu/trac/lemon and install LEMON. You also need [Boost](https://www.boost.org/).

To compile this project, use `g++ -std=c++14 -O2 main.cpp -o main`. You can then run the program using `./main`.

I would like to thank [Jaehyun Park](https://github.com/jaehyunp) for the convenient simplex implementation, which I have modified to use exact rational numbers instead of floating-point numbers.
