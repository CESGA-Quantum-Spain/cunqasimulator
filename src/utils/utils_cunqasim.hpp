#pragma once

#include "types_cunqasim.hpp"

//Flips the bit in position p of the binary expansion of N
inline int flipbit(int N, int p) {
    return N ^ (1 << p);  
}

//Checks if the position p of the binary expansion of N is 0
inline bool is_zero(int N, int p) {
    bool bit = (N & (1 << p)) == 0;
    return bit;
}

//Reverse the first M bits of the integer N
inline int reverse_bits(int N, int M) {
    int result = 0;
    for (int i = 0; i < M; ++i) {
        if (N & (1 << i)) {
            result |= (1 << (M - 1 - i));
        }
    }
    return result | (N & ~((1 << M) - 1));
}
