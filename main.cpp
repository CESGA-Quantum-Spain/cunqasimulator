#include <iostream>
#include <vector>

//#include "quantumcircuit.hpp"

int toggleBit(int N, int p) {
    return N ^ (1 << p);  // XOR with a bitmask
}

int main() {

    size_t position = 0;
    size_t n = 2;
    size_t m =  toggleBit(n, position) ;

    std::cout << m << "\n";
    return 0;
}