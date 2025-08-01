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


int main(int argc, char** argv) {

    QuantumCircuit qc = {
        {
            {"name","h"},
            {"qubits", {0}}
        },
        {
            {"name", "cx"},
            {"qubits", {0,1}}
        },
        /* {
            {"name", "unitary"},
            {"params", {{{0.0, 0.0}, {0.0, -1.0}}, {{0.0, 1.0}, {0.0, 0.0}}}},
            {"qubits", {0}}
        }, */
        {
            {"name","measure"},
            {"qubits", {0}}
        },
        {
            {"name", "measure"},
            {"qubits", {1}}
        }
    };


    Executor qpu(5);
    
    json result = qpu.run(qc, 100);

    std::cout << "Total time: " << result.at("time_taken") << " [s] \n";

    json counts = result.at("counts");
    std::cout << "Counts: " << "\n";
    std::cout << counts.dump(4) << "\n";
    
    return 0;
}