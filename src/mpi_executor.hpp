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

    
    int apply_measure(const std::vector<int>& qubits);
    void apply_gate(const std::string& gate_name, const std::vector<int>& qubits);
    void apply_parametric_gate(const std::string& gate_name, const std::vector<int>& qubits, std::vector<double>& param);
    void apply_unitary(const std::string& instruction_name, const Matrix& matrix, const std::vector<int>& qubits);
    int get_nonzero_position();
    void restart_statevector();
    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);

    int mpi_rank;

private:
    int mpi_size;
    bool mpi_process_needed = true;
    bool is_last_process = false;
    std::unordered_map<int, std::vector<uint64_t>> statevector_ranges;
    uint64_t total_statevector_len;
    uint64_t local_statevector_len;
    uint64_t max_len_on_this_process;
    uint64_t first_element_on_this_process;
    uint64_t last_element_on_this_process;
    
};