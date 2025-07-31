#include "implementations.hpp"

#include <iostream>
#include <complex>
#include <cmath>
#include <numeric>
#include <random>
#include <thread>


#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

int cunqa_apply_measure(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement;

    if (threads) {
        const int n_threads = (1 << n_threads_exponent); 
        std::vector<std::thread> threads_vector;
        std::barrier<> first_barrier(n_threads);
        std::barrier<> second_barrier(n_threads);
        threads_vector.reserve(n_threads);
        std::vector<Precision> partials_prob_0(n_threads);
        std::vector<Precision> partials_prob_1(n_threads);
        Precision prob_0 = 0;
        Precision prob_1 = 0;

        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_measure, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent, std::ref(partials_prob_0), std::ref(partials_prob_1), std::ref(prob_0), std::ref(prob_1), std::ref(measurement), std::ref(first_barrier), std::ref(second_barrier)); 
        }
        for (std::thread& t : threads_vector) {
            t.join();
        }

        threads_vector.clear();

        return measurement;
    } else {
        Precision prob_0 = 0;
        Precision prob_1 = 0;
        std::vector<int> index_0;
        std::vector<int> index_1;

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                index_0.push_back(i);
                index_1.push_back(flipbit(i, qubits[0]));
                prob_0 = prob_0 + std::norm(statevector[i]);
                prob_1 = prob_1 + std::norm(statevector[flipbit(i, qubits[0])]); 
            }
        }

        std::random_device rd;  // Seed source
        std::mt19937 gen(rd() ^ std::chrono::steady_clock::now().time_since_epoch().count()); // Mersenne Twister RNG
        std::discrete_distribution<int> dist({prob_0, prob_1});
        measurement = dist(gen);

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                statevector[i] = (1 - measurement) * (ONE/std::sqrt((measurement + prob_0))) * statevector[i];
                statevector[flipbit(i, qubits[0])] = measurement * (ONE/std::sqrt((1 - measurement + prob_1))) * statevector[flipbit(i, qubits[0])];
            }
        }

        return measurement;
    }
}


// One-Qubit Gates
void cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_x, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); // Blocks until the thread finishes
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = aux;
            }
        }
    }
}


void cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_y, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = imag * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = -imag * aux;
            }
        }
    }
}

void cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_z, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                statevector[flipbit(i, qubits[0])] = -statevector[flipbit(i, qubits[0])];
            }
        }
    }   
}

void cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_h, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux - inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
            }
        }
    }
}

void cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_sx, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        Precision a_half = ONE/(Precision)2;

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = a_half * (ONE + imag) * statevector[i] + a_half * (ONE - imag) * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = a_half * (ONE + imag) * aux + a_half * (ONE - imag) * statevector[flipbit(i, qubits[0])];
            }
        }
    }
}

// Two-Qubit Gates
void cunqa_apply_swap(StateVector& statevector, std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_swap, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        if (qubits[0] > qubits[1]) {
            int aux_qubit_0 = qubits[0];
            qubits[0] = qubits[1];
            qubits[1] = aux_qubit_0;
        }
        std::complex<Precision> aux;
        for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                    aux = statevector[flipbit(k, qubits[0])];
                    statevector[flipbit(k, qubits[0])] = statevector[flipbit(k, qubits[1])];
                    statevector[flipbit(k, qubits[1])] = aux;
                }
            }
        }
    }
}


void cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_cx, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = statevector[flipbit(flipbit(k, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = aux;
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = statevector[flipbit(flipbit(k, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = aux;
                    }
                }
            }
        }
    }
}

void cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_cy, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = imag * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = -imag * aux;
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = imag * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = -imag * aux;
                    }
                }
            }
        }
    }
}

void cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_cz, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = -statevector[flipbit(flipbit(k, qubits[0]), qubits[1])];
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = -statevector[flipbit(flipbit(k, qubits[0]), qubits[1])];
                    }
                }
            }
        }
    }
}

void cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_ecr, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux_00;
        std::complex<Precision> aux_01;
        std::complex<Precision> aux_10;
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        aux_00 = statevector[k];
                        aux_01 = statevector[flipbit(k, qubits[0])];
                        aux_10 = statevector[flipbit(k, qubits[1])];
                        statevector[k] = inverse_sqrt_2 * imag * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] + inverse_sqrt_2 * statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = inverse_sqrt_2 * aux_00 - imag * inverse_sqrt_2 * statevector[flipbit(k, qubits[1])];
                        statevector[flipbit(k, qubits[1])] = imag * inverse_sqrt_2 * aux_01 + inverse_sqrt_2 * statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = -imag * inverse_sqrt_2 * aux_00 + inverse_sqrt_2 * aux_10;
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        aux_00 = statevector[k];
                        aux_01 = statevector[flipbit(k, qubits[0])];
                        aux_10 = statevector[flipbit(k, qubits[1])];
                        statevector[k] = inverse_sqrt_2 * imag * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] + inverse_sqrt_2 * statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = inverse_sqrt_2 * aux_00 - imag * inverse_sqrt_2 * statevector[flipbit(k, qubits[1])];
                        statevector[flipbit(k, qubits[1])] = imag * inverse_sqrt_2 * aux_01 + inverse_sqrt_2 * statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = -imag * inverse_sqrt_2 * aux_00 + inverse_sqrt_2 * aux_10;
                    }
                }
            }
        }
    }
}

//Classical conditional one-qubit gates
void cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_x(statevector, {qubits[1]}, n_qubits, threads);
    }
}

void cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_y(statevector, {qubits[1]}, n_qubits, threads);
    }
}

void cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_z(statevector, {qubits[1]}, n_qubits, threads);
    }
}

void cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_h(statevector, {qubits[1]}, n_qubits, threads);
    }
}

void cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_sx(statevector, {qubits[1]}, n_qubits, threads);
    }
}

//Classical conditional two-qubits gates
void cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_cx(statevector, {qubits[1], qubits[2]}, n_qubits, threads);
    }
}

void cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_cy(statevector, {qubits[1], qubits[2]}, n_qubits, threads);
    }
}

void cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_cz(statevector, {qubits[1], qubits[2]}, n_qubits, threads);
    }
}

void cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_ecr(statevector, {qubits[1], qubits[2]}, n_qubits, threads);
    }
}

void cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_rx, std::ref(statevector), qubits, param, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);

        for (uint64_t j = qubits[0]; j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - qubits[0]; i < j + (1ULL << qubits[0]); i++) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] - imag * sin * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])]  - imag * sin * aux;
            }
        }
    }
}

void cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_ry, std::ref(statevector), qubits, param, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] + sin * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - sin * aux;
            }
        }
    }
}

void cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_rz, std::ref(statevector), qubits, param, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                statevector[i] = (cos - imag * sin) * statevector[i];
                statevector[flipbit(i, qubits[0])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
            }
        }
    }
}

void cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_crx, std::ref(statevector), qubits, param, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - imag * sin * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = cos * statevector[flipbit(i, qubits[0])] - imag * sin * aux;
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - imag * sin * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = cos * statevector[flipbit(i, qubits[0])] - imag * sin * aux;
                    }
                }
            }
        }
    }
}

void cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_cry, std::ref(statevector), qubits, param, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] + sin * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - sin * aux;
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] + sin * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - sin * aux;
                    }
                }
            }
        }
    }
}

void cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_crz, std::ref(statevector), qubits, param, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = (cos - imag * sin) * statevector[flipbit(i, qubits[0])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        aux = statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(k, qubits[0])] = (cos - imag * sin) * statevector[flipbit(i, qubits[0])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
                    }
                }
            }
        }
    }
}

void cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_rx(statevector, {qubits[1]}, param, n_qubits, threads);
    }
}

void cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_ry(statevector, {qubits[1]}, param, n_qubits, threads);
    }
}

void cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_rz(statevector, {qubits[1]}, param, n_qubits, threads);
    }
}

void cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{

    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_1_gate, U, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux;

        for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = U[0][0] * statevector[i] + U[0][1] * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = U[1][0] * aux  + U[1][1] * statevector[flipbit(i, qubits[0])];
            }
        }
    }
}

void cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    if (threads) {
        const int n_threads = (1 << n_threads_exponent);
        std::vector<std::thread> threads_vector;
        threads_vector.reserve(n_threads);
        for (uint16_t i = 0; i < n_threads; i++) {
            threads_vector.emplace_back(apply_thread_2_gate, U, std::ref(statevector), qubits, n_qubits, i, n_threads_exponent); 
        }
        for (std::thread& t : threads_vector) {
            t.join(); 
        }
        threads_vector.clear();
    } else {
        std::complex<Precision> aux_00;
        std::complex<Precision> aux_01;
        std::complex<Precision> aux_10;
        if (qubits[0] > qubits[1]) {
            for (uint64_t j = (1ULL << qubits[0]); j < statevector.size(); j = j + (1ULL << (qubits[0] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[0]) + (1ULL << qubits[1]); i < j; i = i + ((1ULL << (qubits[1] + 1)))) {
                    for (uint64_t k = i - j + (1ULL << qubits[0]) - (1ULL << qubits[1]); k < i; k++) {
                        aux_00 = statevector[k];
                        aux_01 = statevector[flipbit(k, qubits[0])];
                        aux_10 = statevector[flipbit(k, qubits[1])];
                        statevector[k] = U[0][0] * statevector[i] + U[0][1] * statevector[flipbit(k, qubits[0])] + U[1][0] * statevector[flipbit(k, qubits[1])] + U[1][1] * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])]; 
                        statevector[flipbit(k, qubits[0])] = U[1][1] * statevector[flipbit(k, qubits[0])] + U[1][0] * aux_00 + U[1][2] * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] + U[1][3] * statevector[flipbit(k, qubits[1])];
                        statevector[flipbit(k, qubits[1])] = U[2][2] * statevector[flipbit(k, qubits[1])] + U[2][0] * aux_00 + U[2][1] * aux_01 + U[2][3] * statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = U[3][3] * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] + U[3][0] * aux_00 + U[3][1] * aux_01 + U[3][2] * aux_10;
                    }
                }
            }
        } else if (qubits[0] < qubits[1]) {
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]) + (1ULL << qubits[0]); i < j; i = i + ((1ULL << (qubits[0] + 1)))) {
                    for (uint64_t k = i - (1ULL << qubits[0]); k < i; k++) {
                        aux_00 = statevector[k];
                        aux_01 = statevector[flipbit(k, qubits[0])];
                        aux_10 = statevector[flipbit(k, qubits[1])];
                        statevector[k] = U[0][0] * statevector[i] + U[0][1] * statevector[flipbit(k, qubits[0])] + U[1][0] * statevector[flipbit(k, qubits[1])] + U[1][1] * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])]; 
                        statevector[flipbit(k, qubits[0])] = U[1][1] * statevector[flipbit(k, qubits[0])] + U[1][0] * aux_00 + U[1][2] * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] + U[1][3] * statevector[flipbit(k, qubits[1])];
                        statevector[flipbit(k, qubits[1])] = U[2][2] * statevector[flipbit(k, qubits[1])] + U[2][0] * aux_00 + U[2][1] * aux_01 + U[2][3] * statevector[flipbit(k, qubits[0])];
                        statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] = U[3][3] * statevector[flipbit(flipbit(k, qubits[0]), qubits[1])] + U[3][0] * aux_00 + U[3][1] * aux_01 + U[3][2] * aux_10;
                    }
                }
            }
        }
    }
}

void cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_1_gate(U, statevector, {qubits[1]}, n_qubits, threads);
    }
}

void cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent)
{
    int measurement = cunqa_apply_measure(statevector, {qubits[0]}, n_qubits, threads);
    if (measurement == 1) {
        cunqa_apply_2_gate(U, statevector, {qubits[1]}, n_qubits, threads);
    }
}


// Threaded gates
void apply_thread_measure(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent, std::vector<Precision>& partials_prob_0, std::vector<Precision>& partials_prob_1, Precision& prob_0, Precision& prob_1, int& measurement, std::barrier<>& first_barrier, std::barrier<>& second_barrier) 
{
    const int n_threads = (1 << n_threads_exponent);
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent));

    uint64_t initial_position = elements_per_thread * thread;
    Precision local_prob_0 = 0;
    Precision local_prob_1 = 0;

    for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
        if (is_zero(i, qubits[0])) {
            local_prob_0 = local_prob_0 + std::norm(statevector[i]);
        } else {
            local_prob_1 = local_prob_1 + std::norm(statevector[i]);
        }
    }

    partials_prob_0[thread] = local_prob_0;
    partials_prob_1[thread] = local_prob_1;

    first_barrier.arrive_and_wait();

    if (thread == 0) {
        prob_0 = std::accumulate(partials_prob_0.begin(), partials_prob_0.end(), ZERO);
        prob_1 = std::accumulate(partials_prob_1.begin(), partials_prob_1.end(), ZERO);
        std::random_device rd;  // Seed source
        std::mt19937 gen(rd() ^ std::chrono::steady_clock::now().time_since_epoch().count()); // Mersenne Twister RNG
        std::discrete_distribution<int> dist({prob_0, prob_1});
        measurement = dist(gen);
    }

    second_barrier.arrive_and_wait();

    for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
        if (is_zero(i, qubits[0])) {
            statevector[i] = (1 - measurement) * (ONE/std::sqrt(measurement + prob_0)) * statevector[i];
        } else {
            statevector[i] = measurement * (ONE/std::sqrt(1 - measurement + prob_1)) * statevector[i];
        }
    }
}


void apply_thread_x(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent)
{
    std::complex<Precision> aux;
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));

    if (qubits[0] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = aux;
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            aux = statevector[i];
            statevector[i] = statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = aux;
        }
    }
}


void apply_thread_y(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux;
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));

    if (qubits[0] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = imag * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = -imag * aux;
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            aux = statevector[i];
            statevector[i] = imag * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = -imag * aux;
        }
    }
}


void apply_thread_z(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    uint64_t n_positions = (1ULL << (n_qubits - n_threads_exponent));
    uint64_t initial_position = (1ULL << qubits[0]);
    uint64_t thread_initial_position = thread * n_positions + initial_position;
    for (uint64_t position = initial_position; position < n_positions; position++) {
        if (!is_zero(position, qubits[0])) {
            statevector[position] = -statevector[position];
        }
    }
}


void apply_thread_h(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{ 
    std::complex<Precision> aux;
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    
    if (qubits[0] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux - inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            aux = statevector[i];
            statevector[i] = inverse_sqrt_2 * statevector[i] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux - inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
        }
    }
}


void apply_thread_sx(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux;
    Precision a_half = ONE/(Precision)2;
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));

    if (qubits[0] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = a_half * (ONE + imag) * statevector[i] + a_half * (ONE - imag) * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = a_half * (ONE + imag) * aux + a_half * (ONE - imag) * statevector[flipbit(i, qubits[0])];
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            aux = statevector[i];
            statevector[i] = a_half * (ONE + imag) * statevector[i] + a_half * (ONE - imag) * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = a_half * (ONE + imag) * aux + a_half * (ONE - imag) * statevector[flipbit(i, qubits[0])];
        }
    }
}


void apply_thread_swap(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent)
{
    //TODO: Check if this is well implemented
    std::complex<Precision> aux;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (is_zero(i, qubits[0])) {
                    aux = statevector[flipbit(i, qubits[0])];
                    statevector[flipbit(i, qubits[0])] = statevector[flipbit(i, qubits[1])];
                    statevector[flipbit(i, qubits[1])] = aux;
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (is_zero(i, qubits[0])) {
                aux = statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = aux;
            }
        }
    }
}


void apply_thread_cx(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (!is_zero(i, qubits[0])) {
                    aux = statevector[i];
                    statevector[i] = statevector[flipbit(i, qubits[1])];
                    statevector[flipbit(i, qubits[1])] = aux;
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (!is_zero(i, qubits[0])) {
                aux = statevector[i];
                statevector[i] = statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = aux;
            }
        }
    }
}


void apply_thread_cy(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (!is_zero(i, qubits[0])) {
                    aux = statevector[i];
                    statevector[i] = imag * statevector[flipbit(i, qubits[1])];
                    statevector[flipbit(i, qubits[1])] = aux;
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (!is_zero(i, qubits[0])) {
                aux = statevector[i];
                statevector[i] = imag * statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = aux;
            }
        }
    }
}


void apply_thread_cz(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (!is_zero(i, qubits[0])) {
                    statevector[flipbit(i, qubits[1])] = -statevector[flipbit(i, qubits[1])];
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (!is_zero(i, qubits[0])) {
                statevector[flipbit(i, qubits[1])] = -statevector[flipbit(i, qubits[1])];
            }
        }
    }
}


void apply_thread_ecr(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux_00;
    std::complex<Precision> aux_01;
    std::complex<Precision> aux_10;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (is_zero(i, qubits[0])) {
                    aux_00 = statevector[i];
                    aux_01 = statevector[flipbit(i, qubits[0])];
                    aux_10 = statevector[flipbit(i, qubits[1])];
                    statevector[i] = inverse_sqrt_2 * imag * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
                    statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux_00 - imag * inverse_sqrt_2 * statevector[flipbit(i, qubits[1])];
                    statevector[flipbit(i, qubits[1])] = imag * inverse_sqrt_2 * aux_01 + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
                    statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] = -imag * inverse_sqrt_2 * aux_00 + inverse_sqrt_2 * aux_10;
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (is_zero(i, qubits[0])) {
                aux_00 = statevector[i];
                aux_01 = statevector[flipbit(i, qubits[0])];
                aux_10 = statevector[flipbit(i, qubits[1])];
                statevector[i] = inverse_sqrt_2 * imag * statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = inverse_sqrt_2 * aux_00 - imag * inverse_sqrt_2 * statevector[flipbit(i, qubits[1])];
                statevector[flipbit(i, qubits[1])] = imag * inverse_sqrt_2 * aux_01 + inverse_sqrt_2 * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(flipbit(i, qubits[0]), qubits[1])] = -imag * inverse_sqrt_2 * aux_00 + inverse_sqrt_2 * aux_10;
            }
        }
    }
}

void apply_thread_rx(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux;
    Precision sin = std::sin(param[0]/2.0);
    Precision cos = std::cos(param[0]/2.0);
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));

    if (qubits[0] <= n_threads_exponent) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] - imag * sin * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])]  - imag * sin * aux;
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            aux = statevector[i];
            statevector[i] = cos * statevector[i] - imag * sin * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])]  - imag * sin * aux;
        }
    }
}


void apply_thread_ry(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    std::complex<Precision> aux;
    Precision sin = std::sin(param[0]/2.0);
    Precision cos = std::cos(param[0]/2.0);
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));

    if (qubits[0] <= n_threads_exponent) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] + sin * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - sin * aux;
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            aux = statevector[i];
            statevector[i] = cos * statevector[i] + sin * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - sin * aux;
        }
    }
}


void apply_thread_rz(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent) 
{
    Precision sin = std::sin(param[0]/2.0);
    Precision cos = std::cos(param[0]/2.0);
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));

    if (qubits[0] <= n_threads_exponent) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                statevector[i] = (cos - imag * sin) * statevector[i];
                statevector[flipbit(i, qubits[0])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            statevector[i] = (cos - imag * sin) * statevector[i];
            statevector[flipbit(i, qubits[0])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
        }
    }
}

void apply_thread_crx(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent)
{
    std::complex<Precision> aux;
    Precision sin = std::sin(param[0]/2.0);
    Precision cos = std::cos(param[0]/2.0);
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (!is_zero(i, qubits[0])) {
                    aux = statevector[i];
                    statevector[i] = cos * statevector[i] - imag * sin * statevector[flipbit(i, qubits[0])];
                    statevector[flipbit(i, qubits[1])] = cos * statevector[flipbit(i, qubits[0])]  - imag * sin * aux;
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (!is_zero(i, qubits[0])) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] - imag * sin * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[1])] = cos * statevector[flipbit(i, qubits[0])]  - imag * sin * aux;
            }
        }
    }
}


void apply_thread_cry(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent)
{
    std::complex<Precision> aux;
    Precision sin = std::sin(param[0]/2.0);
    Precision cos = std::cos(param[0]/2.0);
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (!is_zero(i, qubits[0])) {
                    aux = statevector[i];
                    statevector[i] = cos * statevector[i] + sin * statevector[flipbit(i, qubits[0])];
                    statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - sin * aux;
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (!is_zero(i, qubits[0])) {
                aux = statevector[i];
                statevector[i] = cos * statevector[i] + sin * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = cos * statevector[flipbit(i, qubits[0])] - sin * aux;
            }
        }
    }
}


void apply_thread_crz(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent)
{
    Precision sin = std::sin(param[0]/2.0);
    Precision cos = std::cos(param[0]/2.0);
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (!is_zero(i, qubits[0])) {
                    statevector[i] = (cos - imag * sin) * statevector[i];
                    statevector[flipbit(i, qubits[0])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
                }
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (!is_zero(i, qubits[0])) {
                statevector[i] = (cos - imag * sin) * statevector[i];
                statevector[flipbit(i, qubits[0])] = (cos + imag * sin) * statevector[flipbit(i, qubits[0])];
            }
        }
    }
}

void apply_thread_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent)
{
    std::complex<Precision> aux;
    uint64_t initial_position;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    
    if (qubits[0] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[0]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[0] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[0]); i < j; i++) {
                aux = statevector[i];
                statevector[i] = U[0][0] * statevector[i] + U[0][1] * statevector[flipbit(i, qubits[0])];
                statevector[flipbit(i, qubits[0])] = U[1][0] * aux  + U[1][1] * statevector[flipbit(i, qubits[0])];
            }
        }
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[0] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            aux = statevector[i];
            statevector[i] = U[0][0] * statevector[i] + U[0][1] * statevector[flipbit(i, qubits[0])];
            statevector[flipbit(i, qubits[0])] = U[1][0] * aux  + U[1][1] * statevector[flipbit(i, qubits[0])];
        }
    }
}


void apply_thread_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent)
{
    std::complex<Precision> aux_00;
    std::complex<Precision> aux_01;
    std::complex<Precision> aux_10;
    uint64_t elements_per_thread = (1ULL << (n_qubits - n_threads_exponent - 1));
    uint64_t initial_position;

    if (qubits[1] <= (n_qubits - n_threads_exponent - 1)) {
        initial_position = (1ULL << (n_qubits - n_threads_exponent)) * thread;
        for (uint64_t j = initial_position + (1ULL << qubits[1]); j < initial_position + 2 * elements_per_thread; j = j + (1ULL << (qubits[1] + 1))) {
            for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                if (is_zero(i, qubits[0])) {
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
    } else {
        initial_position = (1ULL << (n_qubits - n_threads_exponent - 1)) * (2 * thread - (thread % (1ULL << (qubits[1] - n_qubits + n_threads_exponent + 1))));
        for (uint64_t i = initial_position; i < initial_position + elements_per_thread; i++) {
            if (is_zero(i, qubits[0])) {
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