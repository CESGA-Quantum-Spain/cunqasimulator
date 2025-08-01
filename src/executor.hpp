#pragma once

#include <string>
#include <vector>

#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"

class Executor 
{
public:
    int n_qubits;
    StateVector statevector;
    bool with_threads;
    int n_threads_exponent;

    Executor(int n_qubits, bool with_threads = false);
    Executor(StateVector initial_state, bool with_treads = false);

    int apply_measure(const std::vector<int>& qubits);
    void apply_gate(const std::string& gate_name, const std::vector<int>& qubits);
    void apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, Params& param);
    void apply_unitary(const std::string& gate_name, const Matrix& matrix, const std::vector<int>& qubits);
    uint64_t get_nonzero_position();
    void restart_statevector();
    json run(QuantumCircuit& quantumcircuit, int shots = 10);
    
};



