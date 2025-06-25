#include "implementations.hpp"

#include <iostream>
#include <complex>
#include <cmath>
#include <random>

#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

meas_out cunqa_apply_measure(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out output;
    bool zero;
    double prob_0 = 0;
    double prob_1 = 0;
    std::vector<int> index_0;
    std::vector<int> index_1;

    for (uint64_t j = 0; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j; i < j + (1 << qubits[0]); i++) {
            index_0.push_back(i);
            index_1.push_back(flipbit(i, qubits[0]));
            prob_0 = prob_0 + std::norm(statevector[i]);
            prob_1 = prob_1 + std::norm(statevector[flipbit(i, qubits[0])]);
        }
    }

    std::random_device rd;  // Seed source
    std::mt19937 gen(rd() ^ std::chrono::steady_clock::now().time_since_epoch().count()); // Mersenne Twister RNG
    std::discrete_distribution<int> dist({prob_0, prob_1});
    int sample = dist(gen);
    output.measure = sample;

    for (uint64_t j = 0; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j; i < j + (1 << qubits[0]); i++) {
            statevector[i] = (1 - sample) * (1.0/std::sqrt((1 - prob_1))) * statevector[i];
            statevector[flipbit(i, qubits[0])] = sample * (1.0/std::sqrt((1 - prob_0))) * statevector[flipbit(i, qubits[0])];
        }
    }

    output.statevector = statevector;

    return output;
}

// One-Qubit Gates
void cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            aux = statevector[i];
            statevector[i] = statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = aux;
        }
    }
}


void cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            aux = statevector[i];
            statevector[i] = imag * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = -imag * aux;
        }
    }
}


void cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits)
{
    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            statevector[flipbit(i, qubits[0])] = -statevector[flipbit(i, qubits[0])];
        }
    }

}

void cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            aux = statevector[i];
            statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
        }
    }
}


void cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;
    double a_half = (double)1.0/(double)2.0;

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            aux = statevector[i];
            statevector[i] = a_half * (1.0 + imag) * statevector[i] + a_half * (1.0 - imag) * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = a_half * (1.0 + imag) * aux + a_half * (1.0 - imag) * statevector[flipbit(i, qubits[0])];
        }
    }
}

// Two-Qubit Gates
void cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;
    bool zero;

    for (uint64_t j = qubits[1]; j < statevector.size(); j = j + (1 << (qubits[1] + 1))) { // Iterate over target qubit
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[0]);
            if (!zero) {
                aux = statevector[i];
                statevector[i] = statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = aux;
            }
        }
    }
}

void cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;
    bool zero;

    for (uint64_t j = qubits[1]; j < statevector.size(); j = j + (1 << (qubits[1] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[0]);
            if (!zero) {
                aux = statevector[i];
                statevector[i] = imag * statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = -imag * aux;
            }
        }
    }
}

void cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;
    bool zero;

    for (uint64_t j = qubits[1]; j < statevector.size(); j = j + (1 << (qubits[1] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[0]);
            if (!zero) {
                statevector[i] = statevector[i];
                statevector[flipbit(i, qubits[1])] = statevector[flipbit(i, qubits[1])];
            }
        }
    }
}

void cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux_00;
    std::complex<double> aux_01;
    std::complex<double> aux_10;
    bool zero;

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[1]);
            if (zero) {
                aux_00 = statevector[i];
                aux_01 = statevector[flipbit(i, qubits[0])];
                aux_10 = statevector[flipbit(i, qubits[1])];
                statevector[i] = inverse_sqrt_2 * statevector[flipbit(i, qubits[0])] + imag * inverse_sqrt_2 * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux_00 - imag * inverse_sqrt_2 * statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = imag * inverse_sqrt_2 * aux_01 + inverse_sqrt_2 * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] = -imag * inverse_sqrt_2 * aux_00 + inverse_sqrt_2 * aux_10;
            }
        }
    }
}


//Classical conditional one-qubit gates

void cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_x(statevector, {qubits[1]});
    }
}

void cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_y(statevector, {qubits[1]});
    }
}

void cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_z(statevector, {qubits[1]});
    }
}

void cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_h(statevector, {qubits[1]});
    }
}

void cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_sx(statevector, {qubits[1]});
    }
}

//Classical conditional two-qubits gates
void cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_cx(statevector, {qubits[1], qubits[2]});
    }
}

void cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_cy(statevector, {qubits[1], qubits[2]});
    }
}

void cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_cz(statevector, {qubits[1], qubits[2]});
    }
}

void cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_ecr(statevector, {qubits[1], qubits[2]});
    }
}


void cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    std::complex<double> aux;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            aux = statevector[i];
            statevector[i] = cos * statevector[i] - imag * sin * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])]  - imag * sin * aux;
        }
    }
}


void cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    std::complex<double> aux;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            aux = statevector[i];
            statevector[i] = cos * statevector[i] + sin * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] + sin * aux;
        }
    }
}


void cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    std::complex<double> aux;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);

    for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1 << (qubits[0] + 1))) {
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            statevector[i] = (cos - imag * sin) * statevector[i];
            statevector[flipbit(i, qubits[0])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
        }
    }
}

void cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    std::complex<double> aux;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);
    bool zero;

    for (uint64_t j = qubits[1]; j < statevector.size(); j = j + (1 << (qubits[1] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[0]);
            if (!zero) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] - imag * sin * statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = cos * statevector[flipbit(i, qubits[1])] - imag * sin * aux;
            }
        }
    }
}

void cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    std::complex<double> aux;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);
    bool zero;

    for (uint64_t j = qubits[1]; j < statevector.size(); j = j + (1 << (qubits[1] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[0]);
            if (!zero) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] - sin *  statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = cos * statevector[flipbit(i, qubits[1])] + sin * aux;
            }
        }
    }
}

void cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    std::complex<double> aux;
    double sin = std::sin(param[0]/2.0);
    double cos = std::cos(param[0]/2.0);
    bool zero;

    for (uint64_t j = qubits[1]; j < statevector.size(); j = j + (1 << (qubits[1] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[0]);
            if (!zero) {
                statevector[i] = statevector[i] = (cos - imag * sin) * statevector[i];
                statevector[flipbit(i, qubits[1])] = (cos + imag * sin) * statevector[flipbit(i, qubits[1])];
            }
        }
    }
}

void cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_rx(statevector, {qubits[1]}, param);
    }
}

void cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_ry(statevector, {qubits[1]}, param);
    }
}

void cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const Params& param)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_rz(statevector, {qubits[1]}, param);
    }
}


void cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux;

    for (uint64_t j = qubits[1]; j < statevector.size(); j = j + (1 << (qubits[1] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            statevector[i] = U[0][0] * statevector[i] + U[0][1] * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = U[1][1] * statevector[flipbit(i, qubits[0])] + U[1][0] * statevector[i];
        }
    }
}

void cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits)
{
    std::complex<double> aux_00;
    std::complex<double> aux_01;
    std::complex<double> aux_10;
    bool zero;

    for (uint64_t j = qubits[0]; j < statevector.size(); j + (1 << (qubits[1] + 1))) { 
        for (uint64_t i = j - qubits[0]; i < j + (1 << qubits[0]); i++) {
            zero = is_zero(i, qubits[1]);
            if (zero) {
                aux_00 = statevector[i];
                aux_01 = statevector[flipbit(i, qubits[0])];
                aux_10 = statevector[flipbit(i, qubits[1])];
                statevector[i] = U[0][0] * statevector[i] + U[0][1] * statevector[flipbit(i, qubits[0])] + U[1][0] * statevector[flipbit(i, qubits[1])] + U[1][1] * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])]; 

                statevector[flipbit(i, qubits[0])] = U[1][1] * statevector[flipbit(i, qubits[0])] + U[1][0] * aux_00 + U[1][2] * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + U[1][3] * statevector[flipbit(i, qubits[1])];

                statevector[flipbit(i, qubits[1])] = U[2][2] * statevector[flipbit(i, qubits[1])] + U[2][0] * aux_00 + U[2][1] * aux_01 + U[2][3] * statevector[flipbit(i, qubits[0])];

                statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] = U[3][3] * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + U[3][0] * aux_00 + U[3][1] * aux_01 + U[3][2] * aux_10;
            }
        }
    }
}

void cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_1_gate(U, statevector, {qubits[1]});
    }
}

void cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits)
{
    meas_out meas = cunqa_apply_measure(statevector, {qubits[0]});
    if (meas.measure == 1) {
        cunqa_apply_2_gate(U, statevector, {qubits[1]});
    }
}