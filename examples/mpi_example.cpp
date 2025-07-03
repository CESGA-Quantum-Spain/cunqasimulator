#include <iostream>
#include <vector>
#include <complex>
#include <chrono>

#include "mpi_executor.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"


int main() {

    int N_QUBITS = 32;
    try {
        auto start_executor = std::chrono::high_resolution_clock::now();
        MPIExecutor mpi_executor(N_QUBITS);
        auto end_executor = std::chrono::high_resolution_clock::now();
        auto duration_executor = std::chrono::duration_cast<std::chrono::microseconds>(end_executor - start_executor);
        std::cout << "Time taken instanciating executor on process " << mpi_executor.mpi_rank << ": " << duration_executor.count() << " ms" << "\n";
        std::cout.flush();

        auto start_h = std::chrono::high_resolution_clock::now();
        mpi_executor.apply_gate("h", {31});
        auto end_h = std::chrono::high_resolution_clock::now();
        auto duration_h = std::chrono::duration_cast<std::chrono::microseconds>(end_h - start_h);
        std::cout << "Time taken for H Gate on process " << mpi_executor.mpi_rank << ": " << duration_h.count() << " ms" << "\n";
        std::cout.flush();

        /* auto start_cx = std::chrono::high_resolution_clock::now();
        mpi_executor.apply_gate("cx", {34, 35});
        auto end_cx = std::chrono::high_resolution_clock::now();
        auto duration_cx = std::chrono::duration_cast<std::chrono::milliseconds>(end_cx - start_cx);
        std::cout << "Time taken for CX Gate on process " << mpi_executor.mpi_rank << ": " << duration_cx.count() << " ms" << "\n";
        std::cout.flush(); */
        int nonzero_position = mpi_executor.get_nonzero_position();
        std::cout << "NonZeroPosition on process " << mpi_executor.mpi_rank << ": " << nonzero_position << "\n";
        std::cout.flush();

        
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n"; 
    }
    

    return 0;
}