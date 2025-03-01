#pragma once

#include <vector>

using StateVector = StateVector;

StateVector apply_x(StateVector& statevector, int qubits[2])
{
    int n_qubits = statevector.size();

    for (int i = 0; i < n_qubits; i++) {
        statevector[i] = ;
    }

    return statevector;
}

StateVector apply_y(StateVector& statevector, int qubits[2])
{
    int n_qubits = statevector.size();

    return statevector;
}

StateVector apply_z(StateVector& statevector, int qubits[2])
{
    int n_qubits = statevector.size();

    return statevector;
}

StateVector apply_cx(StateVector& statevector, int qubits[2])
{
    int n_qubits = statevector.size();

    return statevector;
}

StateVector apply_cy(StateVector& statevector, int qubits[2])
{
    int n_qubits = statevector.size();

    return statevector;
}

StateVector apply_cz(StateVector& statevector, int qubits[2])
{
    int n_qubits = statevector.size();

    return statevector;
}