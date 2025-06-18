#pragma once

#include <string>
#include <vector>

#include "utils/types_cunqasim.hpp"


class Instruction
{
public:
    
    static void apply_unitary(Matrix& U, StateVector& statevector, std::vector<int> qubits);
    static meas_out apply_measure(StateVector& statevector, std::vector<int> qubits);
    static void apply_instruction(StateVector& statevector, std::string instruction_name, std::vector<int> qubits);
    static void apply_param_instruction(StateVector& statevector, std::string instruction_name, std::vector<int> qubits, Params& param);
    
};





