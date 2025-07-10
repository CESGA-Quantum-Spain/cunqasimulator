#pragma once

#include <vector>

#include "utils/types_cunqasim.hpp"


int cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
// One-Qubit Gates
void cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
// Two-Qubit Gates
void cunqa_apply_swap(StateVector& statevector, std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
//Classical conditional one-qubit gates
void cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
//Classical conditional two-qubits gates
void cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
// Personalized gates
void cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);
void cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, bool threads, const int n_threads_exponent = 0);

// Threaded gates

void apply_thread_measure(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent, std::vector<Precision>& partials_prob_0, std::vector<Precision>& partials_prob_1, Precision& prob_0, Precision& prob_1, int& measurement);
void apply_thread_x(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_y(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_z(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_h(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_sx(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_swap(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_cx(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_cy(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_cz(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_ecr(StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_rx(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_ry(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_rz(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_crx(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_cry(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_crz(StateVector& statevector, const std::vector<int>& qubits, const Params& param, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
void apply_thread_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int>& qubits, const int& n_qubits, const uint16_t& thread, const int n_threads_exponent);
