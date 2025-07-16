#include <iostream>
#include <vector>
#include <complex>

#include "executor.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"

int main() {

    int N_QUBITS = 25;
    bool with_threads = true;

    Executor executor (N_QUBITS, with_threads);

    QuantumCircuit qc = {
        {
            {"name", "h"},
            {"qubits", {0}}
        },
        {
            {"name", "cx"},
            {"qubits", {0, 1}}
        },
        {
            {"name", "measure"},
            {"qubits", {0}}
        },
        {
            {"name", "measure"},
            {"qubits", {1}}
        }
    };

    ResultCunqa result = executor.run(qc, 100);
    json counts  = result.counts;

    std::cout << "Result counts: " << counts.dump() << "\n";
    std::cout << "Time taken: " << result.total_time << "\n"; 



    return 0;
}