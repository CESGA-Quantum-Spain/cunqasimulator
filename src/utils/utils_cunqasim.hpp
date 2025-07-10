#pragma once

#include <cstdlib>

#include "types_cunqasim.hpp"

//Flips the bit in position p of the binary expansion of N
inline uint64_t flipbit(uint64_t N, uint64_t p) {
    return N ^ (1ULL << p);  
}

//Checks if the position p of the binary expansion of N is 0
inline bool is_zero(uint64_t N, uint64_t p) {
    bool bit = (N & (1ULL << p)) == 0;
    return bit;
}

//Reverse the first M bits of the integer N
inline uint64_t reverse_bits(uint64_t N, uint64_t M) {
    uint64_t result = 0;
    for (uint64_t i = 0; i < M; ++i) {
        if (N & (1ULL << i)) {
            result |= (1ULL << (M - 1 - i));
        }
    }
    return result | (N & ~((1ULL << M) - 1));
}