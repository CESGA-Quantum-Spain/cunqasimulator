#include <iostream>
#include <vector>
#include <complex>

#include "executor.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"

namespace {
void printvector (StateVector& sv)
{
    std::cout << "( ";
    for (int i = 0; i < sv.size(); i++) {
        std::cout << sv[i];
        if (i != sv.size() - 1) {
            std::cout << ", ";
        }
    } 
    std::cout << " ) \n";
}
}

int main() {

    int N_QUBITS = 30;
    bool with_threads = true;
    try {
        auto start_executor = std::chrono::high_resolution_clock::now();
        Executor executor(N_QUBITS, with_threads);
        auto end_executor = std::chrono::high_resolution_clock::now();
        auto duration_executor = std::chrono::duration_cast<std::chrono::nanoseconds>(end_executor - start_executor);
        std::cout << "Time taken instanciating executor with " << N_QUBITS << " qubits: " << duration_executor.count() << " nanos" << "\n";
        std::cout.flush();

        auto start_gate = std::chrono::high_resolution_clock::now();
        executor.apply_gate("h", {3});
        auto end_gate = std::chrono::high_resolution_clock::now();
        auto duration_gate = std::chrono::duration_cast<std::chrono::nanoseconds>(end_gate - start_gate);
        std::cout << "Time taken for Gate H with " << N_QUBITS << " qubits: " << duration_gate.count() << " nanos" << "\n";
        /* auto start_cx = std::chrono::high_resolution_clock::now();
        executor.apply_gate("cx", {5, 6});
        auto end_cx = std::chrono::high_resolution_clock::now();
        auto duration_cx = std::chrono::duration_cast<std::chrono::nanoseconds>(end_cx - start_cx);
        std::cout << "Time taken for Gate CX with " << N_QUBITS << " qubits: " << duration_cx.count() << " nanos" << "\n";
        std::cout.flush(); */
        /* auto start_measure = std::chrono::high_resolution_clock::now();
        executor.apply_measure({5});
        auto end_measure = std::chrono::high_resolution_clock::now();
        auto duration_measure = std::chrono::duration_cast<std::chrono::nanoseconds>(end_measure - start_measure);
        std::cout << "Time taken for Measure with " << N_QUBITS << " qubits: " << duration_measure.count() << " nanos" << "\n";
        std::cout.flush(); */
        //printvector(executor.statevector);
        /* uint64_t non_zero_position = executor.get_nonzero_position();
        std::cout << "Nonzero position: " << non_zero_position << "\n"; */
        
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n"; 
    }
    

    return 0;
}