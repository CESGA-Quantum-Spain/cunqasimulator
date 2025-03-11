#pragma once

#include <mpi.h>

#include "types_cunqasim.hpp"

inline int flipbit(int N, int p) {
    return N ^ (1 << p);  
}

inline bool is_zero(int N, int p) {
    bool bit = (N & (1 << p)) == 0;
    return bit;
}

//Reverse the first M bits of the integer M
int reverse_bits(int N, int M) {
    int result = 0;
    for (int i = 0; i < M; ++i) {
        if (N & (1 << i)) {
            result |= (1 << (M - 1 - i));
        }
    }
    return result | (N & ~((1 << M) - 1));
}
inline int get_nonzero_position(StateVector& statevector)
{
    int position;
    auto it = std::find_if(statevector.begin(), statevector.end(), [](const complex& c) {
        return c != std::complex<double>(0, 0); // Check for nonzero
    });

    position = std::distance(statevector.begin(), it);

    return position;
}

inline int get_mpi_rank()
{
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    return mpi_rank;
}