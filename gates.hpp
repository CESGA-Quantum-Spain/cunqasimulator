#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "constants.hpp"
#include "gate_apply.hpp"

using StateVector = std::vector<double>;

const std::vector<std::string> instructions = {"measure", "id", "h", "x", "y", "z", "cx", "cy", "cz"};

class Gate
{
public:
    std::string gate_name;

    Gate(std::string name);
    StateVector apply_gate(StateVector& statevector, int qubits[2]);

};

Gate::Gate(std::string name) 
{
    if (std::find(instructions.begin(), instructions.end(), name) == instructions.end()) {
        std::cout << "Error. Invalid gate name" <<"\n";
    } else {
        this->gate_name = name;
    }
}

StateVector Gate::apply_gate(StateVector& statevector, int qubits[2])
{
    switch (gates_map[this->gate_name])
    {
    case id:
        break;
    case x:
        statevector = apply_x(statevector, qubits);
        break;
    case y:
        statevector = apply_y(statevector, qubits);
        break;
    case z:
        statevector = apply_z(statevector, qubits);
        break;
    case cx:
        statevector = apply_cx(statevector, qubits);
        break;
    case cy:
        statevector = apply_cy(statevector, qubits);
        break;
    case cz:
        statevector = apply_cz(statevector, qubits);
        break;
    
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}
