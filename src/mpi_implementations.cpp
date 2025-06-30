#include "mpi_implementations.hpp"

#include <iostream>
#include <complex>
#include <algorithm>
#include <cmath>
#include <random>
#include <mpi.h>

#include "implementations.hpp"
#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"

namespace 
{
    const uint64_t chunk_size = 128 * 1024 * 1024; // 128MB
    const uint64_t statevector_size_bytes = (1ULL << 35) * 16;
    const uint64_t num_chunks = statevector_size_bytes / chunk_size;
    const uint64_t chunk_elements = chunk_size / 16;
}


meas_out mpi_cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits, const int& mpi_rank) 
{
    StateVector sv;
    meas_out res;
    res.statevector = sv;
    res.measure = 0;
    return res;
}
// One-Qubit Gates
void mpi_cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
    if (qubits[0] <= 35 && mpi_rank == 0) {
        cunqa_apply_x(statevector, qubits);
    } else if (qubits[0] > 35) {
        MPI_Request send_request;
        MPI_Request recv_request;
        std::complex<double>* send_chunk_ptr;
        std::complex<double>* recv_chunk_ptr;
        int comm;
        StateVector aux_statevector(chunk_elements);
        std::complex<double> aux;
        /* int send_completed = 0;
        int recv_completed = 0; */
        MPI_Status status;
        int N = qubits[0] - 35;
        if (mpi_rank < (1 << N)) {
            for (uint64_t i = 0; i < num_chunks; ++i) {
                send_chunk_ptr = statevector.data() + (i * chunk_elements);
                recv_chunk_ptr = aux_statevector.data();
                comm = (mpi_rank + (1 << (N - 1))) % (1 << N);
                MPI_Isend(send_chunk_ptr, chunk_elements, MPI_CXX_DOUBLE_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
                MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_CXX_DOUBLE_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

                MPI_Wait(&send_request, &status); 
                MPI_Wait(&recv_request, &status);

                for (uint64_t j = 0; j < chunk_elements; j++) {
                    statevector[j + (i * chunk_elements)] = recv_chunk_ptr[j];
                }
            }
        }
    }
}


void mpi_cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
    
}


void mpi_cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
  
}


void mpi_cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) 
{
    
}


void mpi_cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
// Two-Qubit Gates
void mpi_cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
//Classical conditional one-qubit gates
void mpi_cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
//Classical conditional two-qubits gates
void mpi_cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
void mpi_cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& mpi_rank) {}
// Personalized gates
void mpi_cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& mpi_rank) {}