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
    #if defined(QPU_MPI) || defined(NO_COMM)
    static inline StateVector apply_instruction(std::string instruction_name, StateVector& statevector, Params& param, std::array<int, 3> qubits, std::array<int, 2>& qpus);
    #elif defined(QPU_ZMQ)
    static inline StateVector apply_instruction(std::string instruction_name, StateVector& statevector, Params& param, std::array<int, 3> qubits, std::array<std::string, 2>& qpus, ZMQSockets& zmq_sockets);
    #endif

};

#if defined(QPU_MPI) || defined(NO_COMM)
inline StateVector Instruction::apply_instruction(std::string instruction_name,StateVector& statevector, Params& param, std::array<int, 3> qubits, std::array<int, 2>& qpus)
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
    //Parametric
    case rx:
        statevector = apply_rx(statevector, param, qubits);
        break;
    case ry:
        statevector = apply_ry(statevector, param, qubits);
        break;
    case rz:
        statevector = apply_rz(statevector, param, qubits);
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
        statevector = apply_cz(statevector, qubits);
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
    case c_if_rx:
        statevector = apply_cifrx(statevector, param, qubits);
        break;
    case c_if_ry:
        statevector = apply_cifry(statevector, param, qubits);
        break;
    case c_if_rz:
        statevector = apply_cifrz(statevector, param, qubits);
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
    //Distributed classical conditional one-qubit gates
    case d_c_if_h:
        statevector = apply_dcifh(statevector, qubits, qpus);
        break;
    case d_c_if_x:
        statevector = apply_dcifx(statevector, qubits, qpus);
        break;
    case d_c_if_y:
        statevector = apply_dcify(statevector, qubits, qpus);
        break;
    case d_c_if_z:
        statevector = apply_dcifz(statevector, qubits, qpus);
        break;
    case d_c_if_rx:
        statevector = apply_dcifrx(statevector, param, qubits, qpus);
        break;
    case d_c_if_ry:
        statevector = apply_dcifry(statevector, param, qubits, qpus);
        break;
    case d_c_if_rz:
        statevector = apply_dcifrz(statevector, param, qubits, qpus);
        break;
    //Distributed classical conditional two-qubit gates
    case d_c_if_cx:
        statevector = apply_dcifcx(statevector, qubits, qpus);
        break;
    case d_c_if_cy:
        statevector = apply_dcifcy(statevector, qubits, qpus);
        break;
    case d_c_if_cz:
        statevector = apply_dcifcz(statevector, qubits, qpus);
        break;
    case d_c_if_ecr:
        statevector = apply_dcifecr(statevector, qubits, qpus);
        break;
    
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}

#elif defined(QPU_ZMQ)
inline StateVector Instruction::apply_instruction(std::string instruction_name, StateVector& statevector, Params& param, std::array<int, 3> qubits, std::array<std::string, 2>& qpus, ZMQSockets& zmq_sockets)
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
    //Parametric
    case rx:
        statevector = apply_rx(statevector, param, qubits);
        break;
    case ry:
        statevector = apply_ry(statevector, param, qubits);
        break;
    case rz:
        statevector = apply_rz(statevector, param, qubits);
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
        statevector = apply_cz(statevector, qubits);
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
    case c_if_rx:
        statevector = apply_cifrx(statevector, param, qubits);
        break;
    case c_if_ry:
        statevector = apply_cifry(statevector, param, qubits);
        break;
    case c_if_rz:
        statevector = apply_cifrz(statevector, param, qubits);
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
    //Distributed classical conditional one-qubit gates
    case d_c_if_h:
        statevector = apply_dcifh(statevector, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_x:
        statevector = apply_dcifx(statevector, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_y:
        statevector = apply_dcify(statevector, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_z:
        statevector = apply_dcifz(statevector, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_rx:
        statevector = apply_dcifrx(statevector, param, qubits,qpus, zmq_sockets);
        break;
    case d_c_if_ry:
        statevector = apply_dcifry(statevector, param, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_rz:
        statevector = apply_dcifrz(statevector, param, qubits, qpus, zmq_sockets);
        break;
    //Distributed classical conditional two-qubit gates
    case d_c_if_cx:
        statevector = apply_dcifcx(statevector, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_cy:
        statevector = apply_dcifcy(statevector, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_cz:
        statevector = apply_dcifcz(statevector, qubits, qpus, zmq_sockets);
        break;
    case d_c_if_ecr:
        statevector = apply_dcifecr(statevector, qubits, qpus, zmq_sockets);
        break;
    
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}

#else

    std::cout << "In CunqaSimulator: No QPU-communication MACRO defined." << "\n";

#endif