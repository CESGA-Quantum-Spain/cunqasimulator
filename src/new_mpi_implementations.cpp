#include "new_mpi_implementations.hpp"

#include <iostream>
#include <complex>
#include <algorithm>
#include <cmath>
#include <random>
#include <mpi.h>

#include "implementations.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"


meas_out new_mpi_cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits, const int& mpi_rank) 
{
    StateVector sv;
    meas_out res;
    res.statevector = sv;
    res.measure = 0;
    return res;
}
// One-Qubit Gates
void new_mpi_cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
    if (qubits[0] < 35 && mpi_rank == 0) {
        cunqa_apply_x(statevector, qubits);
    } else if (qubits[0] > 35) {
        int N = qubits[0] - 35;
        if (mpi_rank == 0) {
            //MPI_Isend(chunk, 1); Chunks of 128MB
            //MPI_Ircv(chunk, 1);
            //MPI_Test()
        } else {
            for (int i = 1; i < N + 1; i++) {
                //MPI_Irecv(chunk, i - 1);
                //MPI_Isend(chunk, i-1);
            }
        }
    }
}


void new_mpi_cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
    
}


void new_mpi_cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
  
}


void new_mpi_cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
    
}


void new_mpi_cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
// Two-Qubit Gates
void new_mpi_cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
//Classical conditional one-qubit gates
void new_mpi_cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
//Classical conditional two-qubits gates
void new_mpi_cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const uint64_t& local_statevector_len, const Params& param, const int& mpi_rank) {}
// Personalized gates
void new_mpi_cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void new_mpi_cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}