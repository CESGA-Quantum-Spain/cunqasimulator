#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <array>
#include <algorithm>

#include "instructions_apply.hpp"
#include "utils/constants_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"


class Instruction
{
public:
    //std::string instruction_name;

    //Instruction(std::string instruction_name);
    static inline StateVector apply_instruction(std::string instruction_name, StateVector& statevector, Params& param, std::array<int, 3> qubits, std::array<int, 2>& qpus, int& n_qubits);

};

/* Instruction::Instruction(std::string instruction_name) 
{
    //TODO: I think this previous check is not needed.
    if (std::find(instructions.begin(), instructions.end(), instruction_name) == instructions.end()) {
        std::cout << "Error. Invalid instruction name" <<"\n";
    } else {
        this->instruction_name = instruction_name;
    }
} */

inline StateVector Instruction::apply_instruction(std::string instruction_name,StateVector& statevector, Params& param, std::array<int, 3> qubits, std::array<int, 2>& qpus, int& n_qubits)
{
    meas_out meas;
    switch (instructions_map[instruction_name])
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
    //Classical conditional one-qubit gates
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
    //Classical conditional two-qubit gates
    case c_if_cx:
        statevector = apply_cifcx(statevector, qubits, n_qubits);
        break;
    case c_if_cy:
        statevector = apply_cifcy(statevector, qubits, n_qubits);
        break;
    case c_if_cz:
        statevector = apply_cifcz(statevector, qubits, n_qubits);
        break;
    case c_if_ecr:
        statevector = apply_cifecr(statevector, qubits, n_qubits);
        break;
    //Distributed classical conditional one-qubit gates
    case d_c_if_h:
        statevector = apply_dcifh(statevector, qubits, n_qubits, qpus);
        break;
    case d_c_if_x:
        statevector = apply_dcifx(statevector, qubits, n_qubits, qpus);
        break;
    case d_c_if_y:
        statevector = apply_dcify(statevector, qubits, n_qubits, qpus);
        break;
    case d_c_if_z:
        statevector = apply_dcifz(statevector, qubits, n_qubits, qpus);
        break;
    case d_c_if_rx:
        statevector = apply_dcifrx(statevector, param, qubits, n_qubits,qpus);
        break;
    case d_c_if_ry:
        statevector = apply_dcifry(statevector, param, qubits, n_qubits, qpus);
        break;
    case d_c_if_rz:
        statevector = apply_dcifrz(statevector, param, qubits, n_qubits, qpus);
        break;
    //Distributed classical conditional two-qubit gates
    case d_c_if_cx:
        statevector = apply_dcifcx(statevector, qubits, n_qubits, qpus);
        break;
    case d_c_if_cy:
        statevector = apply_dcifcy(statevector, qubits, n_qubits, qpus);
        break;
    case d_c_if_cz:
        statevector = apply_dcifcz(statevector, qubits, n_qubits, qpus);
        break;
    case d_c_if_ecr:
        statevector = apply_dcifecr(statevector, qubits, n_qubits, qpus);
        break;
    
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}


