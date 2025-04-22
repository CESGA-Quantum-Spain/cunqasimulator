#pragma once

#include <vector>
#include <complex>
#include <cmath>
#include <random>

#include "utils/constants_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"


inline meas_out cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits)
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
inline void cunqa_apply_h(StateVector& statevector, std::vector<int> qubits)
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
}


inline void cunqa_apply_x(StateVector& statevector, std::vector<int> qubits)
{
    StateVector aux_statevector = statevector;

    for (int i = 0; i < statevector.size(); i++) {
        statevector[i] = aux_statevector[flipbit(i, qubits[0])];
    }
}


inline void cunqa_apply_y(StateVector& statevector, std::vector<int> qubits)
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
}


inline void cunqa_apply_z(StateVector& statevector, std::vector<int> qubits)
{
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == false) { 
            statevector[i] = -statevector[i]; 
        }
    }
}

// Two-Qubit Gates

inline void cunqa_apply_cx(StateVector& statevector, std::vector<int> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (!zero) {
            statevector[i] = aux_statevector[flipbit(i, qubits[1])];
        }
    }
}

inline void cunqa_apply_cy(StateVector& statevector, std::vector<int> qubits)
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
}

inline void cunqa_apply_cz(StateVector& statevector, std::vector<int> qubits)
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
}

inline void cunqa_apply_ecr(StateVector& statevector, std::vector<int> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        zero_1 = is_zero(i, qubits[1]);
        if (zero_0  && zero_1) {
            statevector[i] = inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])] - imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
        } else if (!zero_0 && zero_1) {
            statevector[i] = imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])];
        } else if (zero_0 && !zero_1) {
            statevector[i] = inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])] - imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
        } else if (!zero_0 && !zero_1) {
            statevector[i] = imag * inverse_sqrt_2 * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * aux_statevector[flipbit(i, qubits[0])];
        }
    }
}

//Classical conditional one-qubit gates
inline void cunqa_apply_cifh(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        cunqa_apply_h(statevector, {qubits[0]});
    }
}

inline void cunqa_apply_cifx(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        cunqa_apply_x(statevector, {qubits[0]});
    }
}

inline void cunqa_apply_cify(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        cunqa_apply_y(statevector, {qubits[0]});
    }
}

inline void cunqa_apply_cifz(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        cunqa_apply_z(statevector, {qubits[0]});
    }
}

//Classical conditional two-qubits gates
inline void cunqa_apply_cifcx(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        cunqa_apply_cx(statevector, {qubits[0], qubits[1]});
    }
}

inline void cunqa_apply_cifcy(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        cunqa_apply_cy(statevector, {qubits[0], qubits[1]});
    }
}

inline void cunqa_apply_cifcz(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        cunqa_apply_cz(statevector, {qubits[0], qubits[1]});
    }
}

inline void cunqa_apply_cifecr(StateVector& statevector, std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[2]});
    if (meas.measure == 1) {
        cunqa_apply_ecr(statevector, {qubits[0], qubits[1]});
    }
}


inline void cunqa_apply_rx(StateVector& statevector, std::vector<int> qubits, Params& param)
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
}


inline void cunqa_apply_ry(StateVector& statevector, std::vector<int> qubits, Params& param)
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
}


inline void cunqa_apply_rz(StateVector& statevector, std::vector<int> qubits, Params& param)
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
}



inline void cunqa_apply_cifrx(StateVector& statevector, std::vector<int> qubits, Params& param)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        cunqa_apply_rx(statevector, {qubits[0]}, param);
    }
}

inline void cunqa_apply_cifry(StateVector& statevector, std::vector<int> qubits, Params& param)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        cunqa_apply_ry(statevector, {qubits[0]}, param);
    }
}

inline void cunqa_apply_cifrz(StateVector& statevector, std::vector<int> qubits, Params& param)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[1]});
    if (meas.measure == 1) {
        cunqa_apply_rz(statevector, {qubits[0]}, param);
    }
}


inline void cunqa_apply_1_gate(Matrix& U, StateVector& statevector, std::vector<int> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, qubits[0]);
        if (zero == true) { 
            statevector[i] = U[0][0] * aux_statevector[i] + U[0][1] * aux_statevector[flipbit(i, qubits[0])];
        } else {
            statevector[i] = U[1][1] * aux_statevector[i] + U[1][0] * aux_statevector[flipbit(i, qubits[0])];
        }
    }
}

inline void cunqa_apply_2_gate(Matrix& U, StateVector& statevector, std::vector<int> qubits)
{
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, qubits[0]);
        zero_1 = is_zero(i, qubits[1]);
        if (zero_0  && zero_1) {
            statevector[i] = U[0][0] * aux_statevector[i] + U[0][1] * aux_statevector[flipbit(i, qubits[0])] + U[1][0] * aux_statevector[flipbit(i, qubits[1])] + U[1][1] * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])]; 
        } else if (!zero_0 && zero_1) {
            statevector[i] = U[1][1] * aux_statevector[i] + U[1][0] * aux_statevector[flipbit(i, qubits[0])] + U[1][2] * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + U[1][3] * aux_statevector[flipbit(i, qubits[1])];
        } else if (zero_0 && !zero_1) {
            statevector[i] = U[2][2] * aux_statevector[i] + U[2][0] * aux_statevector[flipbit(i, qubits[1])] + U[2][1] * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + U[2][3] * aux_statevector[flipbit(i, qubits[0])];
        } else if (!zero_0 && !zero_1) {
            U[3][3] * aux_statevector[i] + U[3][0] * aux_statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + U[3][1] * aux_statevector[flipbit(i, qubits[1])] + U[3][2] * aux_statevector[flipbit(i, qubits[0])];
        }
    }
}

