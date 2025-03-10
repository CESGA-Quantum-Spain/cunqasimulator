#pragma once

#include <mpi.h>

#include "types_cunqasim.hpp"

int flipbit(int N, int p) {
    return N ^ (1 << p);  
}

bool is_zero(int N, int p) {
    bool bit = (N & (1 << p)) == 0;
    return bit;
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