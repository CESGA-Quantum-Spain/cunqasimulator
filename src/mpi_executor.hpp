#pragma once

#include <string>
#include <vector>

#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"

class MPIExecutor 
{
public:
    int n_qubits;
    StateVector statevector;

    MPIExecutor(int n_qubits);

    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);
    int apply_measure(std::vector<int>& qubits);
    void apply_unitary(Matrix& matrix, std::vector<int>& qubits);
    void apply_gate(const std::string& gate_name, std::vector<int>& qubits);
    void apply_parametric_gate(const std::string& gate_name, std::vector<int>& qubits, std::vector<double>& param);
    int get_nonzero_position();
    void restart_statevector();

private:
    int mpi_size;
    int mpi_rank;
    uint64_t STATEVECTOR_LEN;
    uint64_t MAX_VECTOR_LEN;
    uint64_t FIRST_STATEVECTOR_ELEMENT;
    uint64_t LAST_STATEVECTOR_ELEMENT;
    uint64_t previous_LAST_ELEMENT;
    
};