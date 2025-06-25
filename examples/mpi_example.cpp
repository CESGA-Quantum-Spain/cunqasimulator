#include <iostream>
#include <vector>
#include <complex>

#include "mpi_executor.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"


int main() {

    int N_QUBITS = 34;
    try {
        MPIExecutor mpi_executor(N_QUBITS);
        
        mpi_executor.apply_gate("x", {16});
        int nonzero_position = mpi_executor.get_nonzero_position();
        std::cout << "NonZeroPosition: " << nonzero_position << "\n";

        
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n"; 
    }
    

    return 0;
}