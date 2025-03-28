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


//#include "logger/logger.hpp"


class Instruction
{
public:
    
    static inline StateVector apply_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits);
    static inline StateVector apply_param_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits, Params& param);


    static inline meas_out apply_measure(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_h(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_x(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_y(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_z(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cx(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cy(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cz(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_ecr(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cifh(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cifx(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cify(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cifz(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cifcx(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cifcy(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cifcz(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_cifecr(StateVector& statevector, std::array<int, 3> qubits);
    static inline StateVector apply_rx(StateVector& statevector, std::array<int, 3> qubits, Params& param);
    static inline StateVector apply_ry(StateVector& statevector, std::array<int, 3> qubits, Params& param);
    static inline StateVector apply_rz(StateVector& statevector, std::array<int, 3> qubits, Params& param);
    static inline StateVector apply_cifrx(StateVector& statevector, std::array<int, 3> qubits, Params& param);
    static inline StateVector apply_cifry(StateVector& statevector, std::array<int, 3> qubits, Params& param);
    static inline StateVector apply_cifrz(StateVector& statevector, std::array<int, 3> qubits, Params& param);


    
};


inline StateVector Instruction::apply_instruction(StateVector& statevector, std::string instruction_name, std::array<int, 3> qubits)
{
    meas_out meas;
    switch (instructions_map[instruction_name])
    {
    //Measure
    case measure:
        meas = Instruction::apply_measure(statevector, qubits);
        statevector = meas.statevector;
        break;
    //One-qubit gates
    case id:
        break;
    case h:
        statevector = Instruction::apply_h(statevector, qubits);
        break;
    case x:
        statevector = Instruction::apply_x(statevector, qubits);
        break;
    case y:
        statevector = Instruction::apply_y(statevector, qubits);
        break;
    case z:
        statevector = Instruction::apply_z(statevector, qubits);
        break;
    //Two-qubit Gates
    case cx:
        statevector = Instruction::apply_cx(statevector, qubits);
        break;
    case cy:
        statevector = Instruction::apply_cy(statevector, qubits);
        break;
    case cz:
        statevector = Instruction::apply_cz(statevector, qubits);
        break;
    case ecr:
        statevector = Instruction::apply_ecr(statevector, qubits);
        break;
    //Classical conditional one-qubit gates
    case c_if_h:
        statevector = Instruction::apply_cifh(statevector, qubits);
        break;
    case c_if_x:
        statevector = Instruction::apply_cifx(statevector, qubits);
        break;
    case c_if_y:
        statevector = Instruction::apply_cify(statevector, qubits);
        break;
    case c_if_z:
        statevector = Instruction::apply_cifz(statevector, qubits);
        break;
        //Classical conditional two-qubit gates
    case c_if_cx:
        statevector = Instruction::apply_cifcx(statevector, qubits);
        break;
    case c_if_cy:
        statevector = Instruction::apply_cifcy(statevector, qubits);
        break;
    case c_if_cz:
        statevector = Instruction::apply_cifcz(statevector, qubits);
        break;
    case c_if_ecr:
        statevector = Instruction::apply_cifecr(statevector, qubits);
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
        statevector = Instruction::apply_rx(statevector, qubits, param);
        break;
    case ry:
        statevector = Instruction::apply_ry(statevector, qubits, param);
        break;
    case rz:
        statevector = Instruction::apply_rz(statevector, qubits, param);
        break;
    case c_if_rx:
        statevector = Instruction::apply_cifrx(statevector, qubits, param);
        break;
    case c_if_ry:
        statevector = Instruction::apply_cifry(statevector, qubits, param);
        break;
    case c_if_rz:
        statevector = Instruction::apply_cifrz(statevector, qubits, param);
        break;
    default:
        std::cout << "Error. Invalid gate name" << "\n";
        break;
    }

    return statevector;
}


inline meas_out Instruction::apply_measure(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out output;
    bool zero;
    double prob_0 = 0;
    double prob_1 = 0;
    std::vector<int> index_0;
    std::vector<int> index_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero) {
            index_0.push_back(i);
            prob_0 = prob_0 + std::norm(statevector[i]);
        } else {
            index_1.push_back(i);
            prob_1 = prob_1 + std::norm(statevector[i]);
        }
    }

    std::random_device rd;  // Seed source
    std::mt19937 gen(rd() ^ std::chrono::steady_clock::now().time_since_epoch().count()); // Mersenne Twister RNG
    std::discrete_distribution<int> dist({prob_0, prob_1});
    int sample = dist(gen);
    output.measure = sample;
    auto it0 = index_0.begin();
    auto it1 = index_1.begin();


    //TODO: The following is parallelizable
    switch (sample)
    {
        case 0:
            for (; it0 != index_0.end() && it1 != index_1.end(); it0++, it1++) {
                statevector[*it0] = (1.0/std::sqrt((1 - prob_1))) * statevector[*it0];
                statevector[*it1] = 0.0;
            }
            break;
        case 1:
            for (; it0 != index_0.end() && it1 != index_1.end(); it0++, it1++) {
                statevector[*it0] = 0.0;
                statevector[*it1] = (1.0/std::sqrt((1 - prob_0))) * statevector[*it1];
            }
            break;
    }

    output.statevector = statevector;

    return output;
}

// One-Qubit Gates
inline StateVector Instruction::apply_h(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = inverse_sqrt_2 * aux_statevector[i] + inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = -inverse_sqrt_2 * aux_statevector[i] + inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])];
        }
    }

    return statevector;
}


inline StateVector Instruction::apply_x(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;

    for (int i = 0; i < statevector.size(); i++) {
        statevector[i] = aux_statevector[flipbit(i, qubits[0])];
    }

    return statevector;
}


inline StateVector Instruction::apply_y(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = imag * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = -imag * aux_statevector[flipbit(i, qubits[0])];
        }
    }

    return statevector;
}


inline StateVector Instruction::apply_z(StateVector& statevector, std::array<int, 3> qubits)
{
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == false) { 
            statevector[i] = -statevector[i]; 
        }
    }

    return statevector;
}

// Two-Qubit Gates

inline StateVector Instruction::apply_cx(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (!zero) {
            statevector[i] = aux_statevector[flipbit(i, qubits[1])];
        }
    }


    return statevector;
}

inline StateVector Instruction::apply_cy(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, qubits[1]);
            if (zero_1) { 
                statevector[i] = imag * aux_statevector[flipbit(i, qubits[1])];
            } else {
                statevector[i] = -imag * aux_statevector[flipbit(i, qubits[1])];
            }
        }
    }

    return statevector;
}

inline StateVector Instruction::apply_cz(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, qubits[1]);
            if (!zero_1) { 
                statevector[i] = -statevector[i]; 
            }
        }
    }

    return statevector;
}

inline StateVector Instruction::apply_ecr(StateVector& statevector, std::array<int, 3> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        zero_1 = is_zero(i, qubits[1]);
        if (zero_0  && zero_1) {
            statevector[i] = inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])] + imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
        } else if (zero_0 && !zero_1) {
            statevector[i] = imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else if (!zero_0 && zero_1) {
            statevector[i] = inverse_sqrt_2 * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])] - imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
        } else if (!zero_0 && !zero_1) {
            statevector[i] = -imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}

//Classical conditional one-qubit gates
inline StateVector Instruction::apply_cifh(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_h(statevector, {qubits[0]});
    }

    return statevector;
}

inline StateVector Instruction::apply_cifx(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_x(statevector, {qubits[0]});
    }

    return statevector;
}

inline StateVector Instruction::apply_cify(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_y(statevector, {qubits[0]});
    }

    return statevector;
}

inline StateVector Instruction::apply_cifz(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_z(statevector, {qubits[0]});
    }

    return statevector;
}

//Classical conditional two-qubits gates
inline StateVector Instruction::apply_cifcx(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_cx(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}

inline StateVector Instruction::apply_cifcy(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_cy(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}

inline StateVector Instruction::apply_cifcz(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_cz(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}

inline StateVector Instruction::apply_cifecr(StateVector& statevector, std::array<int, 3> qubits)
{
    meas_out meas = apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        statevector = apply_ecr(statevector, {qubits[0], qubits[1]});
    }

    return statevector;
}


inline StateVector Instruction::apply_rx(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] - imag * sin * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  - imag * sin * aux_statevector[flipbit(i, qubits[0])];
        }
    }


    return statevector;
}


inline StateVector Instruction::apply_ry(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] + sin * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  +  sin * aux_statevector[flipbit(i, qubits[0])];
        }
    }

    return statevector;
}


inline StateVector Instruction::apply_rz(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    bool zero;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero) { 
            statevector[i] = (cos - imag * sin) * statevector[i];
        } else {
            statevector[i] = (cos + imag * sin) * statevector[i];
        }
    }

    return statevector;
}



inline StateVector Instruction::apply_cifrx(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_rx(statevector, {qubits[0]}, param);
    }

    return statevector;
}

inline StateVector Instruction::apply_cifry(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_ry(statevector, {qubits[0]}, param);
    }

    return statevector;
}

inline StateVector Instruction::apply_cifrz(StateVector& statevector, std::array<int, 3> qubits, Params& param)
{
    meas_out meas = apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        statevector = apply_rz(statevector, {qubits[0]}, param);
    }

    return statevector;
}

