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
    const int max_qubits_per_node = 35;
    const uint64_t chunk_size = 128 * 1024 * 1024; // 128MB
    const uint64_t statevector_size_bytes = (1ULL << max_qubits_per_node) * 16;
    const uint64_t num_chunks = statevector_size_bytes / chunk_size;
    const uint64_t chunk_elements = chunk_size / 16;
}


int mpi_cunqa_apply_measure(StateVector& statevector, std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) 
{
    int measurement;
    return measurement;
}


// One-Qubit Gates
void mpi_cunqa_apply_x(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) 
{
    if (n_qubits <= max_qubits_per_node) {
        if (mpi_rank == 0) {
            cunqa_apply_x(statevector, qubits, n_qubits, false);
        }
    }
    else {
        if (qubits[0] <= max_qubits_per_node ) {
            cunqa_apply_x(statevector, qubits, n_qubits, false);
        } else {
            MPI_Request send_request;
            MPI_Request recv_request;
            MPI_Status status;

            std::complex<Precision>* send_chunk_ptr;
            std::complex<Precision>* recv_chunk_ptr;
            int comm;
            StateVector aux_statevector(chunk_elements);

            //int L = n_qubits - max_qubits_per_node;
            int M = qubits[0] - max_qubits_per_node;
            comm = flipbit(mpi_rank, M - 1);
            for (uint64_t i = 0; i < num_chunks; ++i) {
                send_chunk_ptr = statevector.data() + (i * chunk_elements);
                recv_chunk_ptr = aux_statevector.data();
                
                MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
                MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

                MPI_Wait(&send_request, &status); 
                MPI_Wait(&recv_request, &status);

                for (uint64_t j = 0; j < chunk_elements; j++) {
                    statevector[j + (i * chunk_elements)] = recv_chunk_ptr[j];
                }
            }
        }
    } 
    
}


void mpi_cunqa_apply_y(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) 
{
    if (qubits[0] <= max_qubits_per_node) {
        cunqa_apply_y(statevector, qubits, n_qubits, false);
    } else {
        MPI_Request send_request;
        MPI_Request recv_request;
        MPI_Status status;

        std::complex<Precision>* send_chunk_ptr;
        std::complex<Precision>* recv_chunk_ptr;
        int comm;
        StateVector aux_statevector(chunk_elements);

        //int L = n_qubits - max_qubits_per_node;
        int M = qubits[0] - max_qubits_per_node;
        comm = flipbit(mpi_rank, M - 1);
        for (uint64_t i = 0; i < num_chunks; ++i) {
            send_chunk_ptr = statevector.data() + (i * chunk_elements);
            recv_chunk_ptr = aux_statevector.data();
            MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
            MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

            MPI_Wait(&send_request, &status); 
            MPI_Wait(&recv_request, &status);

            if (is_zero(mpi_rank, M - 1)) {
                for (uint64_t j = 0; j < chunk_elements; j++) {
                    statevector[j + (i * chunk_elements)] = imag * recv_chunk_ptr[j];
                }
            } else {
                for (uint64_t j = 0; j < chunk_elements; j++) {
                    statevector[j + (i * chunk_elements)] = -imag * recv_chunk_ptr[j];
                }
            }
        }        
    }
}


void mpi_cunqa_apply_z(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) 
{

    if (qubits[0] <= max_qubits_per_node) {
        cunqa_apply_z(statevector, qubits, n_qubits, false);
    } else {
        int M = qubits[0] - max_qubits_per_node;
        if(!is_zero(mpi_rank, M - 1)) {
            for (uint64_t i = 0; i < statevector.size(); i++) {
                statevector[i] = -statevector[i];
            }
        }
    }
}


void mpi_cunqa_apply_h(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) 
{
    if (qubits[0] <= max_qubits_per_node) {
        cunqa_apply_h(statevector, qubits, n_qubits, false);
    } else {
        MPI_Request send_request;
        MPI_Request recv_request;
        MPI_Status status;

        std::complex<Precision>* send_chunk_ptr;
        std::complex<Precision>* recv_chunk_ptr;
        int comm;
        StateVector aux_statevector(chunk_elements);

        //int L = n_qubits - max_qubits_per_node;
        int M = qubits[0] - max_qubits_per_node;
        comm = flipbit(mpi_rank, M - 1);

        for (uint64_t i = 0; i < num_chunks; ++i) {
            send_chunk_ptr = statevector.data() + (i * chunk_elements);
            recv_chunk_ptr = aux_statevector.data();
            MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
            MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

            MPI_Wait(&send_request, &status); 
            MPI_Wait(&recv_request, &status);

            if (is_zero(mpi_rank, M - 1)) {
                for (uint64_t j = 0; j < chunk_elements; j++) {
                    statevector[j + (i * chunk_elements)] = inverse_sqrt_2 * statevector[j + (i * chunk_elements)] + inverse_sqrt_2 * recv_chunk_ptr[j];
                }
            } else {
                for (uint64_t j = 0; j < chunk_elements; j++) {
                    statevector[j + (i * chunk_elements)] = inverse_sqrt_2 * recv_chunk_ptr[j] - inverse_sqrt_2 * statevector[j + (i * chunk_elements)];
                }
            }
        }
    }
}


void mpi_cunqa_apply_sx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) 
{
    if (qubits[0] <= max_qubits_per_node) {
        cunqa_apply_sx(statevector, qubits, n_qubits, false);
    } else {
        MPI_Request send_request;
        MPI_Request recv_request;
        MPI_Status status;

        std::complex<Precision>* send_chunk_ptr;
        std::complex<Precision>* recv_chunk_ptr;
        int comm;
        StateVector aux_statevector(chunk_elements);
        Precision a_half = ONE/(Precision)2;

        //int L = n_qubits - max_qubits_per_node;
        int M = qubits[0] - max_qubits_per_node;
        comm = flipbit(mpi_rank, M - 1);
        if (is_zero(mpi_rank, M - 1)) {
            for (uint64_t i = 0; i < num_chunks; ++i) {
                send_chunk_ptr = statevector.data() + (i * chunk_elements);
                recv_chunk_ptr = aux_statevector.data();
                MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
                MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

                MPI_Wait(&send_request, &status); 
                MPI_Wait(&recv_request, &status);

                if (is_zero(mpi_rank, M - 1)) {
                    for (uint64_t j = 0; j < chunk_elements; j++) {
                        statevector[j + (i * chunk_elements)] = a_half * (ONE + imag) * statevector[j + (i * chunk_elements)] + a_half * (ONE - imag) * recv_chunk_ptr[j];
                    }
                } else {
                    for (uint64_t j = 0; j < chunk_elements; j++) {
                        statevector[j + (i * chunk_elements)] = a_half * (ONE + imag) * recv_chunk_ptr[j] + a_half * (ONE - imag) * statevector[j + (i * chunk_elements)];
                    }
                }
            }
        }
    }
}


void mpi_cunqa_apply_rx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) 
{
    if (qubits[0] <= max_qubits_per_node) {
        cunqa_apply_rx(statevector, qubits, param, n_qubits, false);
    } else {
        MPI_Request send_request;
        MPI_Request recv_request;
        MPI_Status status;

        std::complex<Precision>* send_chunk_ptr;
        std::complex<Precision>* recv_chunk_ptr;
        int comm;
        StateVector aux_statevector(chunk_elements);
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);

        //int L = n_qubits - max_qubits_per_node;
        int M = qubits[0] - max_qubits_per_node;
        comm = flipbit(mpi_rank, M - 1);
        for (uint64_t i = 0; i < num_chunks; ++i) {
            send_chunk_ptr = statevector.data() + (i * chunk_elements);
            recv_chunk_ptr = aux_statevector.data();
            MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
            MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

            MPI_Wait(&send_request, &status); 
            MPI_Wait(&recv_request, &status);

            for (uint64_t j = 0; j < chunk_elements; j++) {
                statevector[j + (i * chunk_elements)] = cos * statevector[j + (i * chunk_elements)] - imag * sin * recv_chunk_ptr[j];
            }
        }
    }
}


void mpi_cunqa_apply_ry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) 
{
    if (qubits[0] <= max_qubits_per_node) {
        cunqa_apply_ry(statevector, qubits, param, n_qubits, false);
    } else {
        MPI_Request send_request;
        MPI_Request recv_request;
        MPI_Status status;

        std::complex<Precision>* send_chunk_ptr;
        std::complex<Precision>* recv_chunk_ptr;
        int comm;
        StateVector aux_statevector(chunk_elements);
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);

        //int L = n_qubits - max_qubits_per_node;
        int M = qubits[0] - max_qubits_per_node;
        comm = flipbit(mpi_rank, M - 1);
        if (is_zero(mpi_rank, M - 1)) {
            for (uint64_t i = 0; i < num_chunks; ++i) {
                send_chunk_ptr = statevector.data() + (i * chunk_elements);
                recv_chunk_ptr = aux_statevector.data();
                MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
                MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

                MPI_Wait(&send_request, &status); 
                MPI_Wait(&recv_request, &status);

                if (is_zero(mpi_rank, M - 1)) {
                    for (uint64_t j = 0; j < chunk_elements; j++) {
                        statevector[j + (i * chunk_elements)] = cos * statevector[j + (i * chunk_elements)] + sin * recv_chunk_ptr[j];
                    }
                } else {
                    for (uint64_t j = 0; j < chunk_elements; j++) {
                        statevector[j + (i * chunk_elements)] = cos * statevector[j + (i * chunk_elements)] - sin * recv_chunk_ptr[j];
                    }
                }
            }
        }
    }
}


void mpi_cunqa_apply_rz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) 
{
    if (qubits[0] <= max_qubits_per_node) {
        cunqa_apply_rz(statevector, qubits, param, n_qubits, false);
    } else {
        int M = qubits[0] - max_qubits_per_node;
        Precision sin = std::sin(param[0]/2.0);
        Precision cos = std::cos(param[0]/2.0);
        if (is_zero(mpi_rank, M - 1)) {
            for (uint64_t i = 0; i < statevector.size(); i++) {
                statevector[i] = (cos - imag * sin) * statevector[i];
            }
        } else {
            for (uint64_t i = 0; i < statevector.size(); i++) {
                statevector[i] = (cos + imag * sin) * statevector[i];
            }
        }
    }
}


// Two-Qubit Gates
void mpi_cunqa_apply_cx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) 
{
    //int L = n_qubits - max_qubits_per_node;
    int M = qubits[0] - max_qubits_per_node;
    int N = qubits[1] - max_qubits_per_node;
    int comm;

    if (qubits[0] <= max_qubits_per_node && qubits[1] <= max_qubits_per_node) {
        cunqa_apply_cx(statevector, qubits, n_qubits, false);
    } else if (qubits[0] <= max_qubits_per_node && qubits[1] > max_qubits_per_node) {
        comm = flipbit(mpi_rank, N - 1);
        MPI_Request send_request;
        MPI_Request recv_request;
        MPI_Status status;

        std::complex<Precision>* send_chunk_ptr;
        std::complex<Precision>* recv_chunk_ptr;
        StateVector aux_statevector(chunk_elements);

        for (uint64_t i = 0; i < num_chunks; ++i) {
            send_chunk_ptr = statevector.data() + (i * chunk_elements);
            recv_chunk_ptr = aux_statevector.data();
            MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
            MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

            MPI_Wait(&send_request, &status); 
            MPI_Wait(&recv_request, &status);
            for (uint64_t j = 0; j < chunk_elements; j++) {
                if (!is_zero(j + (i * chunk_elements), qubits[0])) {
                    statevector[j + (i * chunk_elements)] = recv_chunk_ptr[j];
                }
            }
        }
    } else if (qubits[0] > max_qubits_per_node && qubits[1] <= max_qubits_per_node) {
        if (!is_zero(mpi_rank, M - 1)) {
            std::complex<Precision> aux;
            for (uint64_t j = (1ULL << qubits[1]); j < statevector.size(); j = j + (1ULL << (qubits[1] + 1))) {
                for (uint64_t i = j - (1ULL << qubits[1]); i < j; i++) {
                    aux = statevector[i];
                    statevector[i] = statevector[flipbit(i, qubits[1])];
                    statevector[flipbit(i, qubits[1])] = aux;
                }
            }
        }

    } else if (qubits[0] > max_qubits_per_node && qubits[1] > max_qubits_per_node) {
        comm = flipbit(mpi_rank, N - 1);
        MPI_Request send_request;
        MPI_Request recv_request;
        MPI_Status status;

        std::complex<Precision>* send_chunk_ptr;
        std::complex<Precision>* recv_chunk_ptr;
        StateVector aux_statevector(chunk_elements);

        if(!is_zero(mpi_rank, M - 1)) {
            for (uint64_t i = 0; i < num_chunks; ++i) { 
                send_chunk_ptr = statevector.data() + (i * chunk_elements);
                recv_chunk_ptr = aux_statevector.data();
                MPI_Isend(send_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &send_request);
                MPI_Irecv(recv_chunk_ptr, chunk_elements, MPI_COMPLEX, comm, 0, MPI_COMM_WORLD, &recv_request);

                MPI_Wait(&send_request, &status); 
                MPI_Wait(&recv_request, &status);

                for (uint64_t j = 0; j < chunk_elements; j++) {
                    statevector[j + (i * chunk_elements)] = recv_chunk_ptr[j];
                }
            }
        }
    }
}


void mpi_cunqa_apply_cy(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_crx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_crz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_ecr(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
//Classical conditional one-qubit gates
void mpi_cunqa_apply_cifx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cify(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifh(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifsx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifrx(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifry(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifrz(StateVector& statevector, const std::vector<int> qubits, const Params& param, const int& n_qubits, const int& mpi_rank) {}
//Classical conditional two-qubits gates
void mpi_cunqa_apply_cifcx(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifcy(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifcz(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cifecr(StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
// Personalized gates
void mpi_cunqa_apply_1_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_2_gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cif1gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}
void mpi_cunqa_apply_cif2gate(const Matrix& U, StateVector& statevector, const std::vector<int> qubits, const int& n_qubits, const int& mpi_rank) {}