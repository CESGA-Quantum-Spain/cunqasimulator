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

    std::string circuit_str = R"([{"name": "h", "qubits": [0], "params": []}, {"name": "h", "qubits": [1], "params": []}, {"name": "h", "qubits": [2], "params": []}, {"name": "h", "qubits": [3], "params": []}, {"name": "h", "qubits": [4], "params": []}, {"name": "h", "qubits": [5], "params": []}, {"name": "h", "qubits": [6], "params": []}, {"name": "h", "qubits": [7], "params": []}, {"name": "h", "qubits": [8], "params": []}, {"name": "h", "qubits": [9], "params": []}, {"name": "h", "qubits": [10], "params": []}, {"name": "h", "qubits": [11], "params": []}, {"name": "h", "qubits": [12], "params": []}, {"name": "h", "qubits": [13], "params": []}, {"name": "h", "qubits": [14], "params": []}, {"name": "h", "qubits": [15], "params": []}, {"name": "h", "qubits": [16], "params": []}, {"name": "h", "qubits": [17], "params": []}, {"name": "h", "qubits": [18], "params": []}, {"name": "h", "qubits": [19], "params": []}, {"name": "h", "qubits": [20], "params": []}, {"name": "h", "qubits": [21], "params": []}, {"name": "h", "qubits": [22], "params": []}, {"name": "h", "qubits": [23], "params": []}, {"name": "h", "qubits": [24], "params": []}, {"name": "cx", "qubits": [0, 1], "params": []}, {"name": "cx", "qubits": [1, 2], "params": []}, {"name": "cx", "qubits": [2, 3], "params": []}, {"name": "cx", "qubits": [3, 4], "params": []}, {"name": "cx", "qubits": [4, 5], "params": []}, {"name": "cx", "qubits": [5, 6], "params": []}, {"name": "cx", "qubits": [6, 7], "params": []}, {"name": "cx", "qubits": [7, 8], "params": []}, {"name": "cx", "qubits": [8, 9], "params": []}, {"name": "cx", "qubits": [9, 10], "params": []}, {"name": "cx", "qubits": [10, 11], "params": []}, {"name": "cx", "qubits": [11, 12], "params": []}, {"name": "cx", "qubits": [12, 13], "params": []}, {"name": "cx", "qubits": [13, 14], "params": []}, {"name": "cx", "qubits": [14, 15], "params": []}, {"name": "cx", "qubits": [15, 16], "params": []}, {"name": "cx", "qubits": [16, 17], "params": []}, {"name": "cx", "qubits": [17, 18], "params": []}, {"name": "cx", "qubits": [18, 19], "params": []}, {"name": "cx", "qubits": [19, 20], "params": []}, {"name": "cx", "qubits": [20, 21], "params": []}, {"name": "cx", "qubits": [21, 22], "params": []}, {"name": "cx", "qubits": [22, 23], "params": []}, {"name": "cx", "qubits": [23, 24], "params": []}, {"name": "cx", "qubits": [0, 12], "params": []}, {"name": "cx", "qubits": [1, 13], "params": []}, {"name": "cx", "qubits": [2, 14], "params": []}, {"name": "cx", "qubits": [3, 15], "params": []}, {"name": "cx", "qubits": [4, 16], "params": []}, {"name": "cx", "qubits": [5, 17], "params": []}, {"name": "cx", "qubits": [6, 18], "params": []}, {"name": "cx", "qubits": [7, 19], "params": []}, {"name": "cx", "qubits": [8, 20], "params": []}, {"name": "cx", "qubits": [9, 21], "params": []}, {"name": "cx", "qubits": [10, 22], "params": []}, {"name": "cx", "qubits": [11, 23], "params": []}, {"name": "cx", "qubits": [12, 24], "params": []}, {"name": "cx", "qubits": [13, 0], "params": []}, {"name": "cx", "qubits": [14, 1], "params": []}, {"name": "cx", "qubits": [15, 2], "params": []}, {"name": "cx", "qubits": [16, 3], "params": []}, {"name": "cx", "qubits": [17, 4], "params": []}, {"name": "cx", "qubits": [18, 5], "params": []}, {"name": "cx", "qubits": [19, 6], "params": []}, {"name": "cx", "qubits": [20, 7], "params": []}, {"name": "cx", "qubits": [21, 8], "params": []}, {"name": "cx", "qubits": [22, 9], "params": []}, {"name": "cx", "qubits": [23, 10], "params": []}, {"name": "cx", "qubits": [24, 11], "params": []}, {"name": "measure", "qubits": [0], "clbits": [0], "clreg": []}, {"name": "measure", "qubits": [1], "clbits": [1], "clreg": []}, {"name": "measure", "qubits": [2], "clbits": [2], "clreg": []}, {"name": "measure", "qubits": [3], "clbits": [3], "clreg": []}, {"name": "measure", "qubits": [4], "clbits": [4], "clreg": []}, {"name": "measure", "qubits": [5], "clbits": [5], "clreg": []}, {"name": "measure", "qubits": [6], "clbits": [6], "clreg": []}, {"name": "measure", "qubits": [7], "clbits": [7], "clreg": []}, {"name": "measure", "qubits": [8], "clbits": [8], "clreg": []}, {"name": "measure", "qubits": [9], "clbits": [9], "clreg": []}, {"name": "measure", "qubits": [10], "clbits": [10], "clreg": []}, {"name": "measure", "qubits": [11], "clbits": [11], "clreg": []}, {"name": "measure", "qubits": [12], "clbits": [12], "clreg": []}, {"name": "measure", "qubits": [13], "clbits": [13], "clreg": []}, {"name": "measure", "qubits": [14], "clbits": [14], "clreg": []}, {"name": "measure", "qubits": [15], "clbits": [15], "clreg": []}, {"name": "measure", "qubits": [16], "clbits": [16], "clreg": []}, {"name": "measure", "qubits": [17], "clbits": [17], "clreg": []}, {"name": "measure", "qubits": [18], "clbits": [18], "clreg": []}, {"name": "measure", "qubits": [19], "clbits": [19], "clreg": []}, {"name": "measure", "qubits": [20], "clbits": [20], "clreg": []}, {"name": "measure", "qubits": [21], "clbits": [21], "clreg": []}, {"name": "measure", "qubits": [22], "clbits": [22], "clreg": []}, {"name": "measure", "qubits": [23], "clbits": [23], "clreg": []}, {"name": "measure", "qubits": [24], "clbits": [24], "clreg": []}])";

    std::vector<json> circuit = json::parse(circuit_str);

    json result = executor.run(circuit, 100);
    json counts  = result.at("counts");

    std::cout << "Result counts: " << counts.dump() << "\n";
    std::cout << "Time taken: " << result.at("time_taken") << "\n"; 



    return 0;
}