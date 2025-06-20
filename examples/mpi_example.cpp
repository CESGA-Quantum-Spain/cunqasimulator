#include <iostream>
#include <vector>
#include <complex>

#include "mpi_executor.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"


int main() {

    int N_QUBITS = 31;
    try {
        MPIExecutor mpi_executor(N_QUBITS);
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n";
    }
    

    return 0;
}