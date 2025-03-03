#pragma once

#include <vector>
#include <complex>
#include <array>
#include <cmath>
#include <random>


#include "utils.hpp"
#include "constants.hpp"

using complex = std::complex<double>;
using StateVector = std::vector<complex>;


//Measure
StateVector apply_measure(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    bool zero;
    double prob_0 = 0;
    double prob_1 = 0;
    std::vector<int> index_0[(int)statevector.size()/2];
    std::vector<int> index_1[(int)statevector.size()/2];


    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero) {
            index_0->push_back(i);
            prob_0 = prob_0 + std::norm(statevector[i]);
        } else {
            index_1->push_back(i);
            prob_1 = prob_1 + std::norm(statevector[i]);
        }
    }

    std::random_device rd;  // Seed source
    std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::discrete_distribution<int> dist({prob_0, prob_1});
    int sample = dist(gen);
    auto it0 = index_0->begin();
    auto it1 = index_1->begin();

    //TODO: The following is parallelizable
    switch (sample)
    {
        case 0:
            for (; it0 != index_0->end() && it1 != index_1->end(); it0++, it1++) {
                statevector[*it0] = (1.0/std::sqrt((1 - prob_1))) * statevector[*it0];
                statevector[*it1] = 0.0;
            }
            break;
        case 1:
        for (; it0 != index_0->end() && it1 != index_1->end(); it0++, it1++) {
            statevector[*it0] = 0.0;
            statevector[*it1] = (1.0/std::sqrt((1 - prob_0))) * statevector[*it1];
        }
            break;
    }

    return statevector;
}


// One-Qubit Gates
StateVector apply_x(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;

    for (int i = 0; i < statevector.size(); i++) {
        statevector[i] = aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
    }

    return statevector;
}


StateVector apply_y(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = imag * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else {
            statevector[i] = -imag * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_z(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    bool zero; 

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == false) { 
            statevector[i] = -statevector[i]; 
        }
    }

    return statevector;
}


StateVector apply_h(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;
    double ampl = 1.0 / std::sqrt(2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = ampl * aux_statevector[i] + ampl * aux_statevector[flipbit(i, n_qubits - 1 - qubits[0])];
        } else {
            statevector[i] = -ampl * aux_statevector[i] + ampl * aux_statevector[flipbit(i, n_qubits - 1 - qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_rx(StateVector& statevector, double& param, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param/2.0);
    double cos = std::cos(param/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] - imag * sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  - imag * sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_ry(StateVector& statevector, double& param, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;
    double sin = std::sin(param/2.0);
    double cos = std::cos(param/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero == true) { 
            statevector[i] = cos * aux_statevector[i] + sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else {
            statevector[i] = cos * aux_statevector[i]  +  sin * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}


StateVector apply_rz(StateVector& statevector, double& param, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    bool zero;
    double sin = std::sin(param/2.0);
    double cos = std::cos(param/2.0);

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (zero) { 
            statevector[i] = (cos - imag * sin) * statevector[i];
        } else {
            statevector[i] = (cos + imag * sin) * statevector[i];
        }
    }

    return statevector;
}


// Two-Qubit Gates

StateVector apply_cx(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero;

    for (int i = 0; i < statevector.size(); i++) {
        zero = is_zero(i, n_qubits - 1 - qubits[0]);
        if (!zero) {
            statevector[i] = aux_statevector[flipbit(i, n_qubits - 1 - qubits[1])];
        }
    }


    return statevector;
}

StateVector apply_cy(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, n_qubits - 1 - qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, n_qubits - 1 - qubits[1]);
            if (zero_1) { 
                statevector[i] = imag * aux_statevector[flipbit(i, n_qubits - 1 - qubits[1])];
            } else {
                statevector[i] = -imag * aux_statevector[flipbit(i, n_qubits - 1 - qubits[1])];
            }
        }
    }

    return statevector;
}

StateVector apply_cz(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, n_qubits - 1 - qubits[0]);
        if (!zero_0) {
            zero_1 = is_zero(i, n_qubits - 1 - qubits[1]);
            if (!zero_1) { 
                statevector[i] = -statevector[i]; 
            }
        }
    }

    return statevector;
}

StateVector apply_ecr(StateVector& statevector, std::array<int, 2> qubits, int& n_qubits)
{
    //int n_qubits = statevector.size();
    StateVector aux_statevector = statevector;
    double ampl = 1.0 / std::sqrt(2.0);
    bool zero_0;
    bool zero_1;

    for (int i = 0; i < statevector.size(); i++) {
        zero_0 = is_zero(i, n_qubits - 1 - qubits[0]);
        zero_1 = is_zero(i, n_qubits - 1 - qubits[1]);
        if (zero_0  && zero_1) {
            statevector[i] = ampl * aux_statevector[flipbit(i, n_qubits - 1 - qubits[0])] + imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])];
        } else if (zero_0 && !zero_1) {
            statevector[i] = imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])] + ampl * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        } else if (!zero_0 && zero_1) {
            statevector[i] = ampl * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])] - imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])];
        } else if (!zero_0 && !zero_1) {
            statevector[i] = -imag * ampl * aux_statevector[flipbit(flipbit(i, n_qubits - 1 - qubits[0]), n_qubits - 1 - qubits[1])] + ampl * aux_statevector[flipbit(statevector.size() - 1 - i, qubits[0])];
        }
    }

    return statevector;
}