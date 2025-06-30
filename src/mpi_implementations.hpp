#pragma once

#include <vector>

#include "utils/types_cunqasim.hpp"


meas_out mpi_cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits, const int& mpi_rank);
// One-Qubit Gates
void mpi_cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
void mpi_cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
void mpi_cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
// Two-Qubit Gates
void mpi_cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
void mpi_cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
void mpi_cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
void mpi_cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
//Classical conditional one-qubit gates
void mpi_cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
void mpi_cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
void mpi_cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank);
//Classical conditional two-qubits gates
void mpi_cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
// Personalized gates
void mpi_cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);
void mpi_cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank);