#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <array>
#include <nlohmann/json.hpp>

#include "instructions.hpp"
#include "../utils/types.hpp"


class QPU 
{
public:
    int n_qubits;
    StateVector statevector;

    QPU(int n_qubits);
    QPU(StateVector initial_state);

    StateVector apply(std::string gate_name, std::array<int, 3> qubits, double param);
    StateVector run(QuantumCircuit& quantumcircuit);

};

QPU::QPU(int n_qubits) : n_qubits{n_qubits}, statevector(1 << n_qubits)
{
    statevector[0] = 1.0;
}
QPU::QPU(StateVector initial_state) : n_qubits(initial_state.size()), statevector(initial_state)
{}

StateVector QPU::apply(std::string instruction_name, std::array<int, 3> qubits, double param = 0.0)
{
    Instruction instruction(instruction_name);
    
    statevector = instruction.apply_instruction(statevector, param, qubits, this->n_qubits);

    return statevector;
}

//TODO: Shots
//TODO: Classical Registers
StateVector QPU::run(QuantumCircuit& quantumcircuit)
{
    StateVector statevector;
    std::string instruction_name;
    std::array<int, 3> qubits;
    double param;

    for (auto& instruction : quantumcircuit) {

        instruction_name = instruction.at("name");
        qubits = instruction.at("qubits");

        switch (instructions_map[instruction_name])
        {
            case measure:
            case id:
            case x:
            case y:
            case z:
            case h:
            case cx:
            case cy:
            case cz:
            case ecr:
            case c_if_x:
            case c_if_y:
            case c_if_z:
            case c_if_cx:
            case c_if_cy:
            case c_if_cz:
                statevector = this->apply(instruction_name, qubits);
                break;
            case rx:
            case ry:
            case rz:
            case c_if_rx:
            case c_if_ry:
            case c_if_rz:
                param = instruction.at("params");
                statevector = this->apply(instruction_name, qubits, param);
                break;
            default:
                std::cout << "Error. Invalid gate name" << "\n";
                break;
        }
    }

    return statevector;
}
