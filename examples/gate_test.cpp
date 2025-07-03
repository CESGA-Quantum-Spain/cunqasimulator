#include <iostream>
#include <vector>
#include <complex>

#include "executor.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"


int main() {

    int N_QUBITS = 35;
    try {
        auto start_executor = std::chrono::high_resolution_clock::now();
        Executor executor(N_QUBITS);
        auto end_executor = std::chrono::high_resolution_clock::now();
        auto duration_executor = std::chrono::duration_cast<std::chrono::nanoseconds>(end_executor - start_executor);
        std::cout << "Time taken instanciating executor with " << N_QUBITS << " qubits: " << duration_executor.count() << " nanos" << "\n";
        std::cout.flush();

        auto start_gate = std::chrono::high_resolution_clock::now();
        executor.apply_gate("x", {10});
        auto end_gate = std::chrono::high_resolution_clock::now();
        auto duration_gate = std::chrono::duration_cast<std::chrono::nanoseconds>(end_gate - start_gate);
        std::cout << "Time taken for Gate with " << N_QUBITS << " qubits: " << duration_gate.count() << " nanos" << "\n";
        std::cout.flush();
        uint64_t non_zero_position = executor.get_nonzero_position();
        std::cout << "Nonzero position: " << non_zero_position << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n"; 
    }
    

    return 0;
}