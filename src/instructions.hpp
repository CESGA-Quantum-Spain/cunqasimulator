#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <complex>
#include <algorithm>
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
    
    static inline void apply_unitary(Matrix& U, StateVector& statevector, std::vector<int> qubits);
    static inline meas_out apply_measure(StateVector& statevector, std::vector<int> qubits);
    static inline void apply_instruction(StateVector& statevector, std::string instruction_name, std::vector<int> qubits);
    static inline void apply_param_instruction(StateVector& statevector, std::string instruction_name, std::vector<int> qubits, Params& param);
    
    
};

inline void Instruction::apply_unitary(Matrix& U, StateVector& statevector, std::vector<int> qubits)
{
    int dimension = U.size();
    switch(dimension) {
        case 2:
            cunqa_apply_1_gate(U, statevector, qubits);
            break;
        case 4:
            cunqa_apply_2_gate(U, statevector, qubits);
            break;
        default:
            std::cout << "Error. Bad unitary dimension. Only 1-qubit and 2-qubit gates are supported." << "\n";
            break;
    }
}

inline meas_out Instruction::apply_measure(StateVector& statevector, std::vector<int> qubits)
{
    return cunqa_apply_measure(statevector, qubits);
}

inline void Instruction::apply_instruction(StateVector& statevector, std::string instruction_name, std::vector<int> qubits)
{
    switch (instructions_map[instruction_name])
    {
    //One-qubit gates
    case id:
        break;
    case h:
        cunqa_apply_h(statevector, qubits);
        break;
    case x:
        cunqa_apply_x(statevector, qubits);
        break;
    case y:
        cunqa_apply_y(statevector, qubits);
        break;
    case z:
        cunqa_apply_z(statevector, qubits);
        break;
    //Two-qubit Gates
    case cx:
        cunqa_apply_cx(statevector, qubits);
        break;
    case cy:
        cunqa_apply_cy(statevector, qubits);
        break;
    case cz:
        cunqa_apply_cz(statevector, qubits);
        break;
    case ecr:
        cunqa_apply_ecr(statevector, qubits);
        break;
    //Classical conditional one-qubit gates
    case c_if_h:
        cunqa_apply_cifh(statevector, qubits);
        break;
    case c_if_x:
        cunqa_apply_cifx(statevector, qubits);
        break;
    case c_if_y:
        cunqa_apply_cify(statevector, qubits);
        break;
    case c_if_z:
        cunqa_apply_cifz(statevector, qubits);
        break;
        //Classical conditional two-qubit gates
    case c_if_cx:
        cunqa_apply_cifcx(statevector, qubits);
        break;
    case c_if_cy:
        cunqa_apply_cifcy(statevector, qubits);
        break;
    case c_if_cz:
        cunqa_apply_cifcz(statevector, qubits);
        break;
    case c_if_ecr:
        cunqa_apply_cifecr(statevector, qubits);
        break;
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

}

//Parametric
inline void Instruction::apply_param_instruction(StateVector& statevector, std::string instruction_name, std::vector<int> qubits, Params& param)
{
    switch (instructions_map[instruction_name])
    {
    case rx:
        cunqa_apply_rx(statevector, qubits, param);
        break;
    case ry:
        cunqa_apply_ry(statevector, qubits, param);
        break;
    case rz:
        cunqa_apply_rz(statevector, qubits, param);
        break;
    case c_if_rx:
        cunqa_apply_cifrx(statevector, qubits, param);
        break;
    case c_if_ry:
        cunqa_apply_cifry(statevector, qubits, param);
        break;
    case c_if_rz:
        cunqa_apply_cifrz(statevector, qubits, param);
        break;
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

}



