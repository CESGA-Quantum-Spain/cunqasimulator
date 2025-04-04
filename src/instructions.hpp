#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <array>
#include <algorithm>
#include <optional>
#include <cmath>
#include <random>
#include <chrono>

#include "utils/constants_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"
#include "implementations.hpp"


//#include "logger/logger.hpp"


class Instruction
{
public:
    
    static inline StateVector apply_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits);
    static inline StateVector apply_param_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, Params& param);
    static inline meas_out apply_measure(StateVector& statevector, std::array<int, 3> qubits);
    
};

inline meas_out Instruction::apply_measure(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas;

    meas = apply_measure(statevector, qubits);
    statevector = meas.statevector;

    return meas;
}

inline StateVector Instruction::apply_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits)
{
    
    switch (instructions_map[instruction_name])
    {
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
inline StateVector Instruction::apply_param_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, Params& param)
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



