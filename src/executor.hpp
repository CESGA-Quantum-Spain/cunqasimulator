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

    Executor(int n_qubits);
    Executor(StateVector initial_state);

    int apply_measure(const std::vector<int>& qubits);
    void apply_gate(const std::string& gate_name, const std::vector<int>& qubits);
    void apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, std::vector<double>& param);
    void apply_unitary(const std::string& gate_name, const Matrix& matrix, const std::vector<int>& qubits);
    int get_nonzero_position();
    void restart_statevector();
    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);
    
};



