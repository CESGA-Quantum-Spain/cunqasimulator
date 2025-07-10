#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"

class MPIExecutor 
{
public:
    int n_qubits;
    StateVector statevector;

    MPIExecutor(int n_qubits); 
    ~MPIExecutor();

    
    int apply_measure(const std::vector<int>& qubits);
    void apply_gate(const std::string& gate_name, const std::vector<int>& qubits);
    void apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, Params& param);
    void apply_unitary(const std::string& instruction_name, const Matrix& matrix, const std::vector<int>& qubits);
    uint64_t get_nonzero_position();
    void restart_statevector();
    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);

    int mpi_rank;
private:
    int mpi_size;
    
    
};