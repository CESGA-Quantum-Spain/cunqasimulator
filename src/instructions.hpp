#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <array>
#include <algorithm>

#include "instructions_apply.hpp"
#include "../utils/constants.hpp"
#include "../utils/types.hpp"


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
    meas_out meas;
    switch (instructions_map[this->instruction_name])
    {
    //Measure
    case measure:
        meas = apply_measure(statevector, qubits, n_qubits);
        statevector = meas.statevector;
        break;
    //One-qubit gates
    case id:
        break;
    case h:
        statevector = apply_h(statevector, qubits, n_qubits);
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
    case c_if_h:
        statevector = apply_cifh(statevector, qubits, n_qubits);
        break;
    case c_if_x:
        statevector = apply_cifx(statevector, qubits, n_qubits);
        break;
    case c_if_y:
        statevector = apply_cify(statevector, qubits, n_qubits);
        break;
    case c_if_z:
        statevector = apply_cifz(statevector, qubits, n_qubits);
        break;
    case c_if_rx:
        statevector = apply_cifrx(statevector, param, qubits, n_qubits);
        break;
    case c_if_ry:
        statevector = apply_cifry(statevector, param, qubits, n_qubits);
        break;
    case c_if_rz:
        statevector = apply_cifrz(statevector, param, qubits, n_qubits);
        break;
    //TODO: Classical conditional two-qubits gates
    
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}


