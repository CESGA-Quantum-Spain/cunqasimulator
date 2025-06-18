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

    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);
    int apply_measure(std::vector<int>& qubits);
    void apply_unitary(Matrix& matrix, std::vector<int>& qubits);
    void apply_gate(const std::string& gate_name, std::vector<int>& qubits);
    void apply_parametric_gate(const std::string& gate_name, std::vector<int>& qubits, std::vector<double>& param);
    int get_nonzero_position();
    void restart_statevector();
    
};



