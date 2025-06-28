#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"

class NewMPIExecutor 
{
public:
    int n_qubits;
    StateVector statevector;

    NewMPIExecutor(int n_qubits);
    ~NewMPIExecutor();

    
    int apply_measure(const std::vector<int>& qubits);
    void apply_gate(const std::string& gate_name, const std::vector<int>& qubits);
    void apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, std::vector<double>& param);
    void apply_unitary(const std::string& instruction_name, const Matrix& matrix, const std::vector<int>& qubits);
    uint64_t get_nonzero_position();
    void restart_statevector();
    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);

private:
    int mpi_size;
    int mpi_rank;
    
};