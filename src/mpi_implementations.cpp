#include "mpi_implementations.hpp"

#include <complex>
#include <cmath>
#include <random>

#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

meas_out mpi_cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits) 
{
    StateVector sv;
    meas_out res;
    res.statevector = sv;
    res.measure = 0;
    return res;
}
// One-Qubit Gates
void mpi_cunqa_apply_x(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_y(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_z(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_h(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_sx(StateVector& statevector, std::vector<int> qubits) {}
// Two-Qubit Gates
void mpi_cunqa_apply_cx(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cy(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cz(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_ecr(StateVector& statevector, std::vector<int> qubits) {}
//Classical conditional one-qubit gates
void mpi_cunqa_apply_cifx(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cify(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cifz(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cifh(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cifsx(StateVector& statevector, std::vector<int> qubits) {}
//Classical conditional two-qubits gates
void mpi_cunqa_apply_cifcx(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cifcy(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cifcz(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cifecr(StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_rx(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_ry(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_rz(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_crx(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_cry(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_crz(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_cifrx(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_cifry(StateVector& statevector, std::vector<int> qubits, Params& param) {}
void mpi_cunqa_apply_cifrz(StateVector& statevector, std::vector<int> qubits, Params& param) {}
// Personalized gates
void mpi_cunqa_apply_1_gate(Matrix& U, StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_2_gate(Matrix& U, StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cif1gate(Matrix& U, StateVector& statevector, std::vector<int> qubits) {}
void mpi_cunqa_apply_cif2gate(Matrix& U, StateVector& statevector, std::vector<int> qubits) {}