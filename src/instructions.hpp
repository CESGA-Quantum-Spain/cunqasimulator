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
    
    static inline StateVector apply_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits);
    static inline StateVector apply_param_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, double param);
    
    #if defined(QPU_MPI) || defined(QPU_ZMQ)
    static inline StateVector appply_dist_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, type_comm& comm_qpus);
    static inline StateVector appply_dist_param_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, type_comm& comm_qpus, double param);
    #endif
};

inline StateVector Instruction::apply_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits)
{
    meas_out meas;
    switch (instructions_map[instruction_name])
    {
    //Measure
    case measure:
        meas = apply_measure(statevector, qubits);
        statevector = meas.statevector;
        break;
    //One-qubit gates
    case id:
        break;
    case h:
        statevector = apply_h(statevector, qubits);
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
    //Two-qubit Gates
    case cx:
        statevector = apply_cx(statevector, qubits);
        break;
    case cy:
        statevector = apply_cy(statevector, qubits);
        break;
    case cz:
        statevector = apply_cz(statevector, qubits);
        break;
    case ecr:
        statevector = apply_ecr(statevector, qubits);
        break;
    //Classical conditional one-qubit gates
    case c_if_h:
        statevector = apply_cifh(statevector, qubits);
        break;
    case c_if_x:
        statevector = apply_cifx(statevector, qubits);
        break;
    case c_if_y:
        statevector = apply_cify(statevector, qubits);
        break;
    case c_if_z:
        statevector = apply_cifz(statevector, qubits);
        break;
        //Classical conditional two-qubit gates
    case c_if_cx:
        statevector = apply_cifcx(statevector, qubits);
        break;
    case c_if_cy:
        statevector = apply_cifcy(statevector, qubits);
        break;
    case c_if_cz:
        statevector = apply_cifcz(statevector, qubits);
        break;
    case c_if_ecr:
        statevector = apply_cifecr(statevector, qubits);
        break;
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}

//Parametric
inline StateVector apply_param_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, double param)
{
    switch (instructions_map[instruction_name])
    {
    case rx:
        statevector = apply_rx(statevector, qubits, param);
        break;
    case ry:
        statevector = apply_ry(statevector, qubits, param);
        break;
    case rz:
        statevector = apply_rz(statevector, qubits, param);
        break;
    case c_if_rx:
        statevector = apply_cifrx(statevector, qubits, param);
        break;
    case c_if_ry:
        statevector = apply_cifry(statevector, qubits, param);
        break;
    case c_if_rz:
        statevector = apply_cifrz(statevector, qubits, param);
        break;
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}

#if defined(QPU_MPI) || defined(QPU_ZMQ)
//Distributed classical conditional one-qubit gates
static inline StateVector appply_dist_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, type_comm& comm_qpus)
{
    switch (instructions_map[instruction_name])
    {
    case d_c_if_h:
        statevector = apply_dcifh(statevector, qubits, comm_qpus);
        break;
    case d_c_if_x:
        statevector = apply_dcifx(statevector, qubits, comm_qpus);
        break;
    case d_c_if_y:
        statevector = apply_dcify(statevector, qubits, comm_qpus);
        break;
    case d_c_if_z:
        statevector = apply_dcifz(statevector, qubits, comm_qpus);
        break;
    //Distributed classical conditional two-qubit gates
    case d_c_if_cx:
        statevector = apply_dcifcx(statevector, qubits, comm_qpus);
        break;
    case d_c_if_cy:
        statevector = apply_dcifcy(statevector, qubits, comm_qpus);
        break;
    case d_c_if_cz:
        statevector = apply_dcifcz(statevector, qubits, comm_qpus);
        break;
    case d_c_if_ecr:
        statevector = apply_dcifecr(statevector, qubits, comm_qpus);
        break;
    
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}

static inline StateVector appply_dist_param_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, type_comm& comm_qpus, double param)
{
    switch (instructions_map[instruction_name])
    {
    case d_c_if_rx:
        statevector = apply_dcifrx(statevector, qubits, comm_qpus, param);
        break;
    case d_c_if_ry:
        statevector = apply_dcifry(statevector, qubits, comm_qpus, param);
        break;
    case d_c_if_rz:
        statevector = apply_dcifrz(statevector, qubits, comm_qpus, param);
        break;
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}

#endif