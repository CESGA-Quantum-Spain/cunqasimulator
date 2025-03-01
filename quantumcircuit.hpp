#pragma once

#include <iostream>
#include <vector>

#include "gates.hpp"

using StateVector = std::vector<double>;

class QuantumCircuit 
{
public:
    int n_qubits;
    StateVector statevector;

    QuantumCircuit(int n_qubits);

    StateVector apply(std::string gate_name, int qubits[2]);

};

QuantumCircuit::QuantumCircuit(int n_qubits) : n_qubits{n_qubits}, statevector(n_qubits)
{}

StateVector QuantumCircuit::apply(std::string gate_name, int qubits[2])
{
    Gate gate(gate_name);
    
    statevector = gate.apply_gate(statevector, qubits);

    return statevector;
}
