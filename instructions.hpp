#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <array>
#include <algorithm>

#include "constants.hpp"
#include "instructions_apply.hpp"

using complex = std::complex<double>;
using StateVector = std::vector<complex>;

const std::vector<std::string> instructions = {"measure", "id", "h", "x", "y", "z", "cx", "cy", "cz"};

class Instruction
{
public:
    std::string instruction_name;

    Instruction(std::string instruction_name);
    StateVector apply_instruction(StateVector& statevector, double& param, std::array<int, 2> qubits, int& n_qubits);

};

Instruction::Instruction(std::string instruction_name) 
{
    if (std::find(instructions.begin(), instructions.end(), instruction_name) == instructions.end()) {
        std::cout << "Error. Invalid instruction name" <<"\n";
    } else {
        this->instruction_name = instruction_name;
    }
}

StateVector Instruction::apply_instruction(StateVector& statevector, double& param, std::array<int, 2> qubits, int& n_qubits)
{
    switch (instructions_map[this->instruction_name])
    {
    //Measure
    case measure:
        statevector = apply_measure(statevector, qubits, n_qubits);
        break;
    //One-qubit gates
    case id:
        break;
    case x:
        statevector = apply_x(statevector, qubits, n_qubits);
        break;
    case y:
        statevector = apply_y(statevector, qubits, n_qubits);
        break;
    case z:
        statevector = apply_z(statevector, qubits, n_qubits);
        break;
    case h:
        statevector = apply_h(statevector, qubits, n_qubits);
        break;
    //Parametric
    case rx:
        statevector = apply_rx(statevector, param, qubits, n_qubits);
        break;
    case ry:
        statevector = apply_ry(statevector, param, qubits, n_qubits);
        break;
    case rz:
        statevector = apply_rz(statevector, param, qubits, n_qubits);
        break;
    //Two-qubit Gates
    case cx:
        statevector = apply_cx(statevector, qubits, n_qubits);
        break;
    case cy:
        statevector = apply_cy(statevector, qubits, n_qubits);
        break;
    case cz:
        statevector = apply_cz(statevector, qubits, n_qubits);
        break;
    case ecr:
        statevector = apply_cz(statevector, qubits, n_qubits);
        break;
    
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}


